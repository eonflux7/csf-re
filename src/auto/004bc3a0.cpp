#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

// Referenced by the original (vtable-address comparison in the gate); no runtime effect.
[[maybe_unused]] static const auto kGrowArrayObjectVtable = CSF_VTBL(0x0041E5F0);

// Appends two interpolated values to the two parallel arrays of this object:
// the first array receives the constant 12, the second receives the
// interpolated value between a2 and a3 (scaled by the current state counter).
// Returns the bit pattern of the interpolated float.
static int __fastcall S_0041e5f0_AppendInterpolatedValues(S_0041e5f0* self, void* /*edx*/, float a2, float a3)
{
    int stepIndex = GetStateValue(reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 100) + 1;
    float stepFloat = static_cast<float>(stepIndex);
    if (stepIndex < 0)
        stepFloat += g_UnsignedIntegerBias;

    float interpolated = stepFloat * (a3 - a2) * g_ProgressScale + a2;

    // First array: append constant 12.
    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(self, nullptr, static_cast<int>(self->field_000c));
    uint32_t index = self->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_0000 + 4 * index);
    self->field_0008 = index + 1;
    *slot = 12;

    // Second array (sub-object at +0x10): append the interpolated float.
    S_0041e5f0* sub = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (sub->field_0008 >= sub->field_0004)
        S_0041e5f0_GrowArrayCapacity(sub, nullptr, static_cast<int>(sub->field_000c));
    uint32_t subIndex = sub->field_0008;
    float* valueSlot = reinterpret_cast<float*>(sub->field_0000 + 4 * subIndex);
    sub->field_0008 = subIndex + 1;
    *valueSlot = interpolated;

    uint32_t bits;
    std::memcpy(&bits, &interpolated, sizeof(bits));
    return static_cast<int>(bits);
}

HOOK(0x004BC3A0, S_0041e5f0_AppendInterpolatedValues, "auto");
