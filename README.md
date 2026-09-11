# tw-mod-corpse-respawn

When enabled, players who die and click "Release Spirit" respawn as a ghost at their corpse location instead of being teleported to the nearest graveyard.

## Configuration

Copy `conf/tw-mod-corpse-respawn.conf.dist` to your module config directory as `tw-mod-corpse-respawn.conf` and adjust:

```ini
[CorpseRespawn]
CorpseRespawn.Enable = 1
CorpseRespawn.DisableInInstances = 0
```

- `Enable`: set to `0` to disable the module and use the default graveyard behavior.
- `DisableInInstances`: set to `1` to keep the default graveyard respawn inside dungeons and raids. Battlegrounds and arenas are always unaffected.

## How it works

The module registers a `PlayerScript` that hooks `OnPlayerReleasedGhost`. When the player releases their spirit, the module schedules a delayed teleport back to the corpse location.

Because this core does not have an `OnPlayerCanRepopAtGraveyard` hook, the default graveyard teleport still happens first; the module then moves the ghost back to the corpse after a short delay. You may see a brief flicker.

Battlegrounds and arenas are intentionally unaffected.

## Build

```sh
cmake -S . -B build -DMODULES=static
cmake -S . -B build -DMODULES=dynamic
# or per-module:
cmake -S . -B build -DMODULE_TW_MOD_CORPSE_RESPAWN=static
```
