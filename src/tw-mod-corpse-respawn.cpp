#include "ScriptObjects.h"
#include "Config/Config.h"
#include "Corpse.h"
#include "Log.h"
#include "Player.h"

namespace
{
    struct CorpseLocation
    {
        uint32 mapId = 0;
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float orientation = 0.0f;
    };

    bool s_enabled = false;
    bool s_disableInInstances = false;

    void LoadConfig()
    {
        s_enabled = sConfig.GetBoolDefault("CorpseRespawn.Enable", true);
        s_disableInInstances = sConfig.GetBoolDefault("CorpseRespawn.DisableInInstances", false);
    }

    class TwModCorpseRespawnWorldScript : public WorldScript
    {
    public:
        TwModCorpseRespawnWorldScript()
            : WorldScript("tw-mod-corpse-respawn_world", { WORLDHOOK_ON_BEFORE_WORLD_INITIALIZED, WORLDHOOK_ON_AFTER_CONFIG_LOAD })
        {
        }

        void OnBeforeWorldInitialized() override
        {
            sLog.outString("[tw-mod-corpse-respawn] module loaded.");
            LoadConfig();
        }

        void OnAfterConfigLoad(bool /*reload*/) override
        {
            LoadConfig();
        }
    };

    class TwModCorpseRespawnPlayerScript : public PlayerScript
    {
    public:
        TwModCorpseRespawnPlayerScript()
            : PlayerScript("tw-mod-corpse-respawn_player", { PLAYERHOOK_ON_PLAYER_RELEASED_GHOST })
        {
        }

        void OnPlayerReleasedGhost(Player* player) override
        {
            if (!s_enabled)
                return;

            if (!player)
                return;

            // Battlegrounds and arenas have their own respawn rules.
            if (player->InBattleGround() || player->InArena())
                return;

            // Optional: use default graveyard behavior in dungeons and raids.
            if (s_disableInInstances && player->GetMap() && player->GetMap()->IsDungeon())
                return;

            Corpse* corpse = player->GetCorpse();
            if (!corpse)
                return;

            CorpseLocation const loc = {
                corpse->GetMapId(),
                corpse->GetPositionX(),
                corpse->GetPositionY(),
                corpse->GetPositionZ(),
                corpse->GetOrientation()
            };

            // Delay the teleport so the default graveyard teleport can complete first.
            player->m_Events.AddLambdaEventAtOffset([player, loc]()
            {
                if (!player || !player->IsInWorld())
                    return;

                if (!player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
                    return;

                player->TeleportTo(loc.mapId, loc.x, loc.y, loc.z, loc.orientation, TELE_TO_NOT_UNSUMMON_PET);

                sLog.outString("[tw-mod-corpse-respawn] respawned player %s at corpse location (map %u, %.2f, %.2f, %.2f).",
                               player->GetName(), loc.mapId, loc.x, loc.y, loc.z);
            }, 500);
        }
    };
}

void Addtw_mod_corpse_respawnScripts()
{
    new TwModCorpseRespawnWorldScript();
    new TwModCorpseRespawnPlayerScript();
}
