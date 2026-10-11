#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies four dwords from a2 into the object's fields 0x08..0x14, after the
// vtable and secondary-table stores that set up the embedded sub-object at +0x04.
// Returns this (the original returns &this->__vftable, which is the same address).
static uint32_t* __fastcall C_00863760_CopyFromDwordArray(C_00863760* self, void* /*edx*/, uint32_t* a2) {
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x0082D428);
    self->vtbl = CSF_VTBL(0x00863760);
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x00863744);

    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0.0f;

    self->field_0008 = a2[0];
    self->field_000c = a2[1];
    self->field_0010 = a2[2];

    // The original stores a2[3] as a raw dword (no int-to-float conversion),
    // so copy the bit pattern.
    uint32_t rawFourth = a2[3];
    std::memcpy(&self->field_0014, &rawFourth, sizeof(float));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0070BAC0, C_00863760_CopyFromDwordArray, "auto");
