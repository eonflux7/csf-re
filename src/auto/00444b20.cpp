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

// Position-vector accessor: ecx = object, no stack args, returns float* in eax.
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

// Alternate-control test: pushes one int, cdecl, returns a byte-sized bool in al.
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));

// Distance threshold global (float) compared against the squared-root distance.
CSF_GLOBAL(float, g_MinimapZoomWide, 0x00830E4C);

// Notifies a color animation event between two objects and bumps the
// counters at +0x44 (events), +0x48 (checked pairs), +0x4C (far pairs).
static void __fastcall WeaponFirearms_NotifyColorAnimation(void* self, void* /*edx*/,
                                                           C_008342e8* a2, C_008342e8* a3)
{
    if (!a3->field_0050) {
        CSF_AT(uint32_t, self, 0x44) += 1;

        if (a2) {
            if (!static_cast<uint8_t>(IsAlternateControlKind(static_cast<int>(a2->field_0050)))) {
                CSF_AT(uint32_t, self, 0x48) += 1;

                float* sourcePos = GetObjectPositionVector(a2, nullptr);
                float* targetPos = GetObjectPositionVector(a3, nullptr);

                float deltaY = targetPos[1] - sourcePos[1];
                float deltaZ = targetPos[2] - sourcePos[2];
                float deltaX = targetPos[0] - sourcePos[0];
                float distance = std::sqrt(deltaZ * deltaZ + deltaY * deltaY + deltaX * deltaX);

                if (distance > g_MinimapZoomWide) {
                    CSF_AT(uint32_t, self, 0x4C) += 1;
                }
            }
        }
    }
}

HOOK(0x444B20, WeaponFirearms_NotifyColorAnimation, "auto");
