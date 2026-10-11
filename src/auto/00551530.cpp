#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_FUNC(int, __cdecl, sub_551490, 0x00551490, ());

// Returns true when the spawn-point state object and the character select state are both
// valid and the team-mode probe reports result 3 or 1. Returns true (default) when either
// pointer chain is missing.
static bool __fastcall SpawnPoints_IsSabotageSpawnReady(void* self, void* /*edx*/) {
    bool result = true;

    uint8_t* stateObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0xC8C));
    if (stateObj == nullptr || g_CharacterSelectState8B0694 == 0) {
        return result;
    }

    uint32_t rawSpawnData = CSF_AT(uint32_t, stateObj, 0x18);
    uint32_t spawnDataAddr = rawSpawnData ? rawSpawnData - 0x6B0 : 0;

    uint32_t rawTeamData = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(g_CharacterSelectState8B0694), 4);
    uint32_t teamDataAddr = rawTeamData ? rawTeamData - 0x308 : 0;

    if (spawnDataAddr != 0 && teamDataAddr != 0) {
        if (sub_551490() == 3) {
            return true;
        }
        if (sub_551490() == 1) {
            return true;
        }
        return false;
    }
    return result;
}

HOOK(0x00551530, SpawnPoints_IsSabotageSpawnReady, "auto");
