#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx, int arg));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

using DeliverObjectPositionFn = int(__fastcall*)(void* self, void* edx, int positionVector);

// Resolves the handle stored at owner+0x3BC in the current scene and, if it exists,
// forwards its position vector to the vtable slot 0xAC of the owner's motion sub-object.
// Returns the owner pointer when the handle index is negative, or 0 when resolution fails.
static int __fastcall AiGiveObject_DeliverObjectPosition(int* self, void* /*edx*/)
{
    uint32_t ownerAddr = CSF_AT(uint32_t, self, 0x08);
    int result = static_cast<int>(ownerAddr);
    uint8_t* owner = reinterpret_cast<uint8_t*>(ownerAddr);

    if (CSF_AT(int, owner, 0x3C4) >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(ownerAddr + 0x3BC));
        if (result != 0) {
            // The original reloads the owner from [self+8] after the Resolve call.
            uint32_t ownerNow = CSF_AT(uint32_t, self, 0x08);
            uint8_t* ownerObj = reinterpret_cast<uint8_t*>(ownerNow);
            uint8_t* motionObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, ownerObj, 0x14));
            void* v3 = reinterpret_cast<void*>(CSF_AT(uint32_t, motionObj, 0x30));
            void** vtbl = CSF_AT(void**, v3, 0);

            int positionVector = GetObjectPositionVector(
                reinterpret_cast<void*>(static_cast<uint32_t>(result)), nullptr, 1);

            return reinterpret_cast<DeliverObjectPositionFn>(vtbl[0xAC / 4])(v3, nullptr, positionVector);
        }
    }
    return result;
}

HOOK(0x004D4960, AiGiveObject_DeliverObjectPosition, "auto");
