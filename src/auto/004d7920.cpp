#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

// Constructor for TLanzarObjetoAPos: initializes the TCharTask base, the
// embedded sub-objects at +0x14 and +0x28, and zeroes the three trailing fields.
// Returns the object address (eax = this).
static uint32_t* __fastcall TLanzarObjetoAPos_Construct(TLanzarObjetoAPos* self, void* /*edx*/)
{
    CharTask_InitBase(self, nullptr);
    CSF_AT(void*, self, 0x00) = reinterpret_cast<void*>(CSF_VTBL(0x0083DAE8));
    CSF_AT(uint32_t, self, 0x04) = reinterpret_cast<uint32_t>(reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083DAC8)));

    // First embedded sub-object at +0x14.
    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    CSF_AT(void*, self, 0x14) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083D748));
    CSF_AT(void*, self, 0x18) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083D728));

    // Second embedded sub-object at +0x28.
    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    CSF_AT(void*, self, 0x28) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083D800));
    CSF_AT(void*, self, 0x2C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083D7E0));

    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D7920, TLanzarObjetoAPos_Construct, "auto");
