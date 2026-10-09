#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Copies the flag byte at offset 1 of the object referenced by a3[1] into the
// object resolved from a3's handle, at offset 0x4D. Returns the resolved field
// value (0 when the handle or target pointer is missing).
static int __stdcall SetSceneObjectFlagFromRef(int /*unusedA1*/, int /*unusedA2*/, uint32_t* refPtr)
{
    // v3 = *(*a3 + 4)
    uint32_t objectAddr = refPtr[0];
    uint32_t innerAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddr), 4);

    // v4 = *(v3 + 12), signed compare below
    int handleIndex = CSF_AT(int, reinterpret_cast<void*>(innerAddr), 12);

    uint32_t result = innerAddr + 4;
    if (handleIndex >= 0)
    {
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(result));
        result = static_cast<uint32_t>(resolved);
        if (result != 0)
        {
            result = CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x3C);
            if (result != 0)
            {
                uint32_t sourceAddr = refPtr[1];
                uint32_t sourceInner = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceAddr), 4);
                uint8_t sourceFlag = CSF_AT(uint8_t, reinterpret_cast<void*>(sourceInner), 1);
                CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x4D) = sourceFlag;
            }
        }
    }
    return static_cast<int>(result);
}

HOOK(0x00460450, SetSceneObjectFlagFromRef, "auto");
