#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original global: PTank plugin offset within the owning object (0x8B09EC).
CSF_GLOBAL(int32_t, g_PTankPluginOffset, 0x8B09EC);

// Scales the object's two stored float components by `scale` and writes them into the
// PTank plugin block (+0xB8, +0xBC), then sets flag 0x4000 at +0x40 of that block.
static int __fastcall C_00862110_ApplyScaledVelocity(C_00862110* self, void* /*edx*/, float scale)
{
    int pluginBase = *reinterpret_cast<int*>(
        self->field_0004 + static_cast<uint32_t>(g_PTankPluginOffset));

    float scaledX = *reinterpret_cast<float*>(&self->field_0028) * scale;
    float scaledY = *reinterpret_cast<float*>(&self->field_002c) * scale;

    CSF_AT(float, pluginBase, 184) = scaledX;
    CSF_AT(float, pluginBase, 188) = scaledY;

    int result = *reinterpret_cast<int*>(
        self->field_0004 + static_cast<uint32_t>(g_PTankPluginOffset));
    CSF_AT(uint32_t, result, 0x40) |= 0x4000u;
    return result;
}

HOOK(0x006EEB40, C_00862110_ApplyScaledVelocity, "auto");
