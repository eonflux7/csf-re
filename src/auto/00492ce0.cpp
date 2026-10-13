#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char*, __fastcall, Task_GetTargetNameBuffer, 0x004D1510, (void* self, void* edx));

// Returns the cached count (self+0x208) of scene entries that have flag bit 1 set,
// whose task target name buffer is non-empty, and whose name-buffer id matches self+0x20.
// The count is recomputed only when the cached value is negative.
static int16_t __fastcall GetMatchingSceneTargetCount(void* self, void* /*edx*/)
{
    if (CSF_AT(int16_t, self, 0x208) >= 0)
        return CSF_AT(int16_t, self, 0x208);

    CSF_AT(int16_t, self, 0x208) = 0;

    uint8_t* scene = static_cast<uint8_t*>(g_pCurrentScene);
    uint32_t entryCount = CSF_AT(uint32_t, scene, 0x58);

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        uint32_t* entryTable = CSF_AT(uint32_t*, scene, 0x50);
        uint32_t entryAddress = entryTable[index];
        if (entryAddress == 0)
            continue;

        uint8_t* entry = reinterpret_cast<uint8_t*>(entryAddress);
        if ((CSF_AT(uint8_t, entry, 0x94) & 1) == 0)
            continue;

        void* targetObject = CSF_AT(void*, entry, 0x34);
        char* nameBuffer = Task_GetTargetNameBuffer(targetObject, nullptr);
        if (*nameBuffer == 0)
            continue;

        if (CSF_AT(uint32_t, nameBuffer, 0x14) == CSF_AT(uint32_t, self, 0x20))
            ++CSF_AT(int16_t, self, 0x208);
    }

    return CSF_AT(int16_t, self, 0x208);
}

HOOK(0x492CE0, GetMatchingSceneTargetCount, "auto");
