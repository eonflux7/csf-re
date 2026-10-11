#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Task object layout (as used here):
//   +0x04..+0x0C : cached target position (x, y, z)
//   +0x10        : handle (passed by address to CHandleTable::Resolve)
//   +0x18        : signed flag / handle state; target is refreshed only when >= 0
static uint32_t* __fastcall Task_GetTargetPosition(uint32_t* self, void* /*edx*/)
{
    if (CSF_AT(int32_t, self, 0x18) >= 0)
    {
        // The original pushes the address of this+0x10 as the handle argument.
        int handleArg = static_cast<int>(reinterpret_cast<intptr_t>(reinterpret_cast<uint8_t*>(self) + 0x10));
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleArg);
        void* targetObject = reinterpret_cast<void*>(static_cast<intptr_t>(resolved));
        if (targetObject)
        {
            uint32_t* positionVector = GetObjectPositionVector(targetObject, nullptr);
            CSF_AT(uint32_t, self, 0x04) = positionVector[0];
            CSF_AT(uint32_t, self, 0x08) = positionVector[1];
            CSF_AT(uint32_t, self, 0x0C) = positionVector[2];
        }
    }
    return self + 1;
}

HOOK(0x004D1520, Task_GetTargetPosition, "auto");
