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

// Base-class constructor: AiAutonomous_ConstructTaskBase (0x004D1660), __thiscall, ret 4.
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (C_0083b958* self, void* /*edx*/, int a2));

// Constructs a C_0083bc28 (autonomous task) in place: runs the base task constructor,
// installs the vtables, and initializes the task state fields.
static uint32_t* __fastcall C_0083bc28_Construct(C_0083bc28* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(reinterpret_cast<C_0083b958*>(self), nullptr, a2);

    self->vtbl = CSF_VTBL(0x0083BC28);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x0083BC08);

    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x0082D448);

    self->field_0068 = 0xFFFFFFFFu;
    self->field_006c = 0xFFFFFFFFu;

    self->field_0040 = 0.0f;
    self->field_0048 = 0;
    self->field_0054 = 0;
    self->field_0044 = 1;
    self->field_004c = 2;
    self->field_0050 = 0xFFFFFFFFu;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D3AD0, C_0083bc28_Construct, "auto");
