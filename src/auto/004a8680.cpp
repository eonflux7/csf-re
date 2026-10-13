#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// C_00834fa8 construction chain: sets vtables, then runs the base init on each embedded sub-object.
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

static int __fastcall C_00834fa8_Construct(C_00834fa8* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x00834FA8);
    self->field_0004 = (uint32_t)CSF_ADDR_VT(0x00834F8C);

    CSF_AT(void*, self, 0x100) = CSF_VTBL(0x0082D410);

    // Embedded sub-objects at +0xEC, +0xD8, +0xC4, +0xB0, +0x9C, +0x78, +0x64, +0x40, +0x28, +0x14, then self.
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0xEC, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0xD8, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0xC4, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0xB0, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x9C, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x78, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x64, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x40, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0((uint8_t*)self + 0x14, nullptr);

    // Final call is a tail jump in the original; the return value is not meaningful.
    CharTaskBase_Init_4DB3C0(self, nullptr);
    return 0;
}

HOOK(0x004A8680, C_00834fa8_Construct, "auto");
