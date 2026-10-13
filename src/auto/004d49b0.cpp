#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void*, float a2, char a3));

// Looks up the target object referenced by this owner's handle. If it resolves, computes the
// angle of the vector from the owner's position to the target's position and starts the
// owner's facing task (C_0082f9f8) with that angle.
static int __fastcall GiveObject_StartFacingTarget(void* self, void* /*edx*/)
{
    uint32_t ownerRaw = CSF_AT(uint32_t, self, 0x08);
    int result = static_cast<int>(ownerRaw);
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ownerRaw));

    if (CSF_AT(int, owner, 0x3C4) >= 0)
    {
        int handle = static_cast<int>(ownerRaw + 0x3BC);
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
        if (result)
        {
            C_008342e8* target = reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(result));

            float* ownerPosition = GetObjectPositionVector(CSF_AT(void*, owner, 0x14), nullptr);
            float* targetPosition = GetObjectPositionVector(target, nullptr);

            alignas(8) uint8_t vectorBytes[12];
            CSF_AT(float, vectorBytes, 0x0) = targetPosition[0] - ownerPosition[0];
            CSF_AT(float, vectorBytes, 0x4) = targetPosition[1] - ownerPosition[1];
            CSF_AT(float, vectorBytes, 0x8) = targetPosition[2] - ownerPosition[2];

            S_00526d40* direction = reinterpret_cast<S_00526d40*>(vectorBytes);
            float angleRadians = static_cast<float>(S_00526d40_GetVectorAngleRadians(direction, nullptr));

            // Owner pointer is reloaded after the calls above, as in the original.
            uint8_t* ownerNow = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
            void* ownerObject = CSF_AT(void*, ownerNow, 0x14);
            C_0082f9f8* facingTask = reinterpret_cast<C_0082f9f8*>(CSF_AT(void*, ownerObject, 0x30));

            return C_0082f9f8_Start(facingTask, nullptr, angleRadians, 1);
        }
    }
    return result;
}

HOOK(0x4D49B0, GiveObject_StartFacingTarget, "auto");
