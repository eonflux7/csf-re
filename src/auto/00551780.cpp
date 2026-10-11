#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int64_t, __fastcall, sub_6472B0, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, sub_6483D0, 0x006483D0, (uint32_t* a1));

// Returns the remaining delay of the spawn timer stored in the spawn-point
// object (this+0x1404 handle), or -1 when no valid spawn entry is available.
static int __fastcall MpSpawn_GetRemainingSpawnDelay(int* self, void* /*edx*/)
{
    int result = -1;
    if (CSF_AT(int, self, 0x140C) >= 0)
    {
        int handleEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                               (int)((uint8_t*)self + 0x1404));
        if (handleEntry)
        {
            int entryPointer = CSF_AT(int, handleEntry, 0x18);
            if (entryPointer)
            {
                int spawnObject = entryPointer - 0x80;
                if (spawnObject)
                {
                    if (CSF_AT(uint8_t, spawnObject, 0x44))
                    {
                        int timerValue = CSF_AT(int, spawnObject, 0x68);
                        int secondaryValue = CSF_AT(int, spawnObject, 0x6C);

                        // Stack object at ebp-8 (8 bytes): callee fills it, slot [1] is ebp-4.
                        uint32_t callBuffer[2] = {0, 0};
                        callBuffer[1] = (uint32_t)secondaryValue;
                        uint32_t* bufferResult = sub_6483D0(&callBuffer[0]);
                        int64_t elapsed = sub_6472B0(bufferResult, nullptr);
                        result = timerValue - (int)elapsed;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x551780, MpSpawn_GetRemainingSpawnDelay, "auto");
