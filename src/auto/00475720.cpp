#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Vtable of S_0041e5f0, referenced by the two GrowArrayCapacity call sites.
[[maybe_unused]] static void* const kGrowArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static C_00831698* __fastcall C_00831698_Construct(C_00831698* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00831698);

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;

    S_0041e5f0* v2 = reinterpret_cast<S_0041e5f0*>(&self->field_0004);
    v2->field_000c = 10;

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 10;

    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 10;

    self->field_0024 = 0;

    uint32_t v3 = v2->field_0004;
    if (v3 < 10)
        S_0041e5f0_GrowArrayCapacity(v2, nullptr, (int)(10 - v3));

    uint32_t v4 = self->field_0018;
    if (v4 < 10)
        S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(&self->field_0014), nullptr, (int)(10 - v4));

    return self;
}

HOOK(0x475720, C_00831698_Construct, "auto");
