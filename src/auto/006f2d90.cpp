#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860a30_InitBase, 0x006CA660, (C_00860a30* self, void*));

static C_00862640* __fastcall C_00862640_Construct(C_00862640* self, void* /*edx*/)
{
    C_00860a30_InitBase(self, nullptr);

    self->vtbl = CSF_VTBL(0x00862640);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00862638));
    self->field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0086261C));

    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_0050 = 0;
    self->field_004c = 0xFFFFFFFFu;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_003c = 0.0f;
    self->field_0040 = 0.0f;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_0034 = 1.0f;
    self->field_0038 = 1.0f;

    return self;
}

HOOK(0x006F2D90, C_00862640_Construct, "auto");
