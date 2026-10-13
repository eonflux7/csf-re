#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original calls (bound by address, as in the packet)
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));

// Sets the current animation on the hierarchy's interpolator when an animation id is
// stored, refreshes the hierarchy matrices, then flags each node in the node table.
// Stack args a2/a3 are unused by the original (ret 8).
static int __fastcall C_00863430_RefreshAnimAndMarkNodes(C_00863430* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    self->field_0014 &= ~0x20u;

    if (self->field_0008)
    {
        void* hierarchy = reinterpret_cast<void*>(self->field_0004);
        RtAnimInterpolatorSetCurrentAnim(
            reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, hierarchy, 0x20)),
            reinterpret_cast<uint32_t*>(self->field_0008));
        RpHAnimHierarchyUpdateMatrices(reinterpret_cast<int*>(self->field_0004));
        self->field_0014 = self->field_0014 | 8u;
        self->field_0018 = 0;
    }

    int nodeCount = CSF_AT(int, reinterpret_cast<void*>(self->field_0004), 4);
    uint32_t result = self->field_000c;
    if (result)
    {
        for (int i = 0; i < nodeCount; ++i)
        {
            result = reinterpret_cast<uint32_t*>(self->field_000c)[i];
            if (result)
                CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0x20) = 1;
        }
    }
    return static_cast<int>(result);
}

HOOK(0x00708660, C_00863430_RefreshAnimAndMarkNodes, "auto");
