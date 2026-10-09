#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_45A900, 0x0045A900, (void* self, void* edx, float* a2, float a3));
CSF_FUNC(int, __fastcall, sub_48E430, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, sub_6AB930, 0x006AB930, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_RadToDegFactor, 0x008313E4);

static int __stdcall ComputeFacingAngleDegreesToTarget(int a1, int a2, uint32_t* a3)
{
    (void)a2;
    void* holder = reinterpret_cast<void*>(a1);

    // *(*(a1 + 4) + 4) = 0
    CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(uint32_t, holder, 4)), 4) = 0;

    // v3 = *(*a3 + 4); v4 = *(v3 + 12); result = v3 + 4
    uint32_t* sourceRef = reinterpret_cast<uint32_t*>(*a3);
    uint32_t infoNode = CSF_AT(uint32_t, sourceRef, 4);
    int32_t componentPriority = CSF_AT(int32_t, reinterpret_cast<void*>(infoNode), 12);
    int result = static_cast<int>(infoNode + 4);

    if (componentPriority >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            // v6 = *(float**)(a3[1] + 4)
            float* targetPos = reinterpret_cast<float*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4));

            // v7 = sub_48E430(result)
            float* sourcePos = reinterpret_cast<float*>(static_cast<uintptr_t>(
                static_cast<uint32_t>(sub_48E430(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result))),
                    nullptr))));

            float deltaVec[3];
            deltaVec[0] = targetPos[1] - sourcePos[0];
            deltaVec[1] = targetPos[2] - sourcePos[1];
            deltaVec[2] = targetPos[3] - sourcePos[2];

            result = sub_45A900(static_cast<void*>(deltaVec), nullptr, &g_ZeroVectorX, 1e-6f);
            if ((result & 0xFF) == 0)
            {
                double vectorMeasure = sub_6AB930(static_cast<void*>(deltaVec), nullptr);
                float angleDegrees = static_cast<float>(vectorMeasure * g_RadToDegFactor);

                // The original reloads the output holder after the call.
                uint32_t outRec = CSF_AT(uint32_t, holder, 4);
                CSF_AT(float, reinterpret_cast<void*>(outRec), 4) = angleDegrees;
                result = static_cast<int>(CSF_AT(uint32_t, holder, 4));
            }
        }
    }

    return result;
}

HOOK(0x0045C1D0, ComputeFacingAngleDegreesToTarget, "auto");
