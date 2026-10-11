#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e9d0_QueryInteractionTarget(C_0083e9d0* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(a3);
    uint8_t* src = reinterpret_cast<uint8_t*>(a2);

    CSF_AT(int, out, 4) = -1;
    int target = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
    if (target
        && ((CSF_AT(uint32_t, target, 0x170) & 0x2000) != 0
            || (uint8_t)GameConfig_IsResolvedEntryActive(CSF_AT(int, target, 0x50)))) {
        if (self->vSlot9C(a2, 14)) {
            CSF_AT(int, out, 4) = 3;
            CSF_AT(int, out, 8) = 14;
            CSF_AT(int, out, 0x18) = CSF_AT(int, src, 4);
            CSF_AT(int, out, 0x1C) = CSF_AT(int, src, 8);

            float* pos = GetObjectPositionVector(reinterpret_cast<void*>(target), nullptr);
            CSF_AT(float, out, 0x20) = pos[0];
            CSF_AT(float, out, 0x24) = pos[1];
            CSF_AT(float, out, 0x28) = pos[2];

            float* targetPos = GetObjectPositionVector(reinterpret_cast<void*>(target), nullptr);
            float* selfPos = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0008), nullptr);

            // The original keeps the intermediate values in x87 registers (double-like precision).
            double dx = (double)selfPos[0] - (double)targetPos[0];
            double dy = (double)selfPos[1] - (double)targetPos[1];
            double dz = (double)selfPos[2] - (double)targetPos[2];
            double distSq = dz * dz + dx * dx;
            distSq = distSq + dy * dy;
            CSF_AT(float, out, 0x2C) = (float)distSq;

            double range = (double)*reinterpret_cast<float*>(&self->field_0034);
            if (distSq < range * range) {
                CSF_AT(int, out, 4) = -1;
            }
        }
    }
    return CSF_AT(int, out, 4);
}

HOOK(0x004E5770, C_0083e9d0_QueryInteractionTarget, "auto");
