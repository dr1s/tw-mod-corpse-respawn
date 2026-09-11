# Optional CMake hooks for tw-mod-corpse-respawn.
#
# Most modules do not need custom CMake here. Source files under src/ are discovered automatically,
# conf/*.conf.dist files are copied automatically, and Addtw_mod_corpse_respawnScripts() is invoked by the
# generated module loader.
#
# This file is included twice:
#   TORTOISE_MODULE_CMAKE_PHASE=DISCOVERY
#   TORTOISE_MODULE_CMAKE_PHASE=POST_TARGETS
#
# Use TW_* helpers for compatibility with this core's module system. Do not use AzerothCore AC_* names.

if(TORTOISE_MODULE_CMAKE_PHASE STREQUAL "DISCOVERY")
  # Example for legacy-style extra script sources:
  # TW_ADD_SCRIPTS("${CMAKE_CURRENT_LIST_DIR}/src/legacy")
  # TW_ADD_SCRIPT_LOADER(tw_mod_corpse_respawn "legacy_loader.h")
endif()
