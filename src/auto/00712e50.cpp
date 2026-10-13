#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00712E50, CInputBase::<ctor-like setup>, __thiscall, ret 8.
// Ensures the embedded array at +0x08 has capacity for at least 5 entries,
// then records 5 at +0x14 and reports success.

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

[[maybe_unused]] static void* const kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

static bool __fastcall CInputBase_EnsureFiveSlotBuffer(CInputBase* self, void* /*edx*/,
                                                       int /*unusedA2*/, int /*unusedA3*/)
{
    uint32_t currentCapacity = self->field_000c;
    if (currentCapacity < 5) {
        S_0041e5f0* buffer = reinterpret_cast<S_0041e5f0*>(&self->field_0008);
        S_0041e5f0_GrowArrayCapacity(buffer, nullptr, static_cast<int>(5 - currentCapacity));
    }
    self->field_0014 = 5;
    return true;
}

HOOK(0x712E50, CInputBase_EnsureFiveSlotBuffer, "auto");
