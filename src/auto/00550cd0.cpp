#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, DMSpawnEntry_Apply, 0x00744E50,
         (void* self, void* edx, char a2, char a3, char a4, char a5, char a6, char a7, char a8));

// Resolves the handle stored at request+8 in the current scene's handle table. If the entry
// is live (flag at +0x50 clear) and carries a non-placeholder object at +0x18 (not the
// sentinel 0x6B0 offset base), applies the spawn with the request's parameter block.
static char __stdcall SpawnPoints_ApplyFromHandle(int32_t* a1)
{
    int result = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                      static_cast<int>(reinterpret_cast<uintptr_t>(a1 + 2)));
    if (result != 0)
    {
        uint8_t* entryBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));
        if (CSF_AT(int, entryBase, 0x50) == 0)
        {
            result = CSF_AT(int, entryBase, 0x18);
            if (result != 0 && result != 1712)
            {
                uint8_t* spawnObj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result - 1712));
                char applied = DMSpawnEntry_Apply(spawnObj, nullptr,
                                                  static_cast<char>(a1[19]),
                                                  static_cast<char>(a1[13]),
                                                  static_cast<char>(a1[14]),
                                                  static_cast<char>(a1[15]),
                                                  static_cast<char>(a1[16]),
                                                  static_cast<char>(a1[17]),
                                                  static_cast<char>(a1[18]));
                result = (result & ~0xFF) | static_cast<uint8_t>(applied);
            }
        }
    }
    return static_cast<char>(result);
}

HOOK(0x00550CD0, SpawnPoints_ApplyFromHandle, "auto");
