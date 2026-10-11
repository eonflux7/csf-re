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

// Base-class constructor (TCharTask init), called with this in ecx.
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

// Constructs an AiAutonomous task object on top of a TCharTask base and returns it as a
// pointer to the object's first dword (its vtable slot).
static uint32_t* __fastcall AiAutonomous_ConstructTaskBase(C_0083b958* self, void* /*edx*/, int a2)
{
    CharTask_InitBase(self, nullptr);

    self->vtbl = CSF_VTBL(0x0083B958);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_VTBL(0x0083B938);
    self->field_0018 = 2;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x0082D448);
    self->field_0030 = 0xFFFFFFFFu;
    self->field_0034 = 0xFFFFFFFFu;
    self->field_001c = 0;
    self->field_0014 = (uint32_t)a2;
    self->field_003a = 0;
    self->field_0038 = 1;
    self->field_0039 = 1;
    self->field_003c = 0xFFFFFFFFu;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D1660, AiAutonomous_ConstructTaskBase, "auto");
