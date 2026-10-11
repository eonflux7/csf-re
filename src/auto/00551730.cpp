#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

// Returns true when the spawn count for the given team slot (1 or 2) has reached
// the limit stored in the global config block (+0x1AC). Unknown slots return false.
static bool __stdcall MpSpawn_HasTeamReachedSpawnLimit(int teamSlot) {
  uint8_t* teamState = nullptr;
  if (g_CharacterSelectState8B0694 != 0) {
    uint32_t stateRef = CSF_AT(uint32_t,
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(g_CharacterSelectState8B0694)), 4);
    if (stateRef != 0) {
      teamState = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(stateRef - 0x308));
    }
  }

  int teamCount;
  if (teamSlot == 1) {
    teamCount = CSF_AT(int, teamState, 0x44);
  } else if (teamSlot == 2) {
    teamCount = CSF_AT(int, teamState, 0x50);
  } else {
    return false;
  }

  uint8_t* globalConfig =
      reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_558150()));
  return teamCount >= CSF_AT(int, globalConfig, 0x1AC);
}

HOOK(0x00551730, MpSpawn_HasTeamReachedSpawnLimit, "auto");
