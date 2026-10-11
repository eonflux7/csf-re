#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_0082d448 constructor-like initializer: copies two dwords from a source record
// (a2+4, a2+8) and a third value (a3) into the object, after setting both slots to -1.
// Returns the object pointer.
static C_0082d448* __fastcall C_0082d448_Initialize(C_0082d448* self, void* /*edx*/, int a2, int a3)
{
    self->field_0004 = 0xFFFFFFFFu;
    self->field_0008 = 0xFFFFFFFFu;
    self->vtbl = CSF_VTBL(0x0082D448);

    void* source = reinterpret_cast<void*>(a2);
    self->field_0004 = CSF_AT(uint32_t, source, 0x4);
    self->field_0008 = CSF_AT(uint32_t, source, 0x8);
    self->field_000c = static_cast<uint32_t>(a3);

    return self;
}

HOOK(0x00405A50, C_0082d448_Initialize, "auto");
