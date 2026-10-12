#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the difficulty label string at index `difficultyIndex` from the
// original g_psz_FACIL table (a dword array of string pointers in .data).
static char* __cdecl GameConfig_GetDifficultyName(int difficultyIndex)
{
    char* const* difficultyNameTable = reinterpret_cast<char* const*>(CSF_ADDR_DATA(0x008A97A8));
    return difficultyNameTable[difficultyIndex];
}

HOOK(0x007614A0, GameConfig_GetDifficultyName, "auto");
