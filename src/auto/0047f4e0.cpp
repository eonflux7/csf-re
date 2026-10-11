#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TAtacarRafaga_Initialize(TAtacarRafaga* self, void* /*edx*/)
{
    CharTask_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x00832F68);
    self->field_0004 = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832F48));

    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    CSF_AT(uint32_t, self, 0x14) = static_cast<uint32_t>(CSF_ADDR_DATA(0x008328F0));
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(CSF_ADDR_DATA(0x008328D0));

    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    CSF_AT(uint32_t, self, 0x28) = static_cast<uint32_t>(CSF_ADDR_DATA(0x008329A8));
    CSF_AT(uint32_t, self, 0x2C) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832988));

    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x3C, nullptr);
    CSF_AT(uint32_t, self, 0x3C) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832A60));
    CSF_AT(uint32_t, self, 0x40) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832A40));

    CharTask_InitBase(reinterpret_cast<uint8_t*>(self) + 0x50, nullptr);
    CSF_AT(uint32_t, self, 0x50) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832B18));
    CSF_AT(uint32_t, self, 0x54) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00832AF8));

    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_008c = 1;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    CSF_AT(void*, self, 0x88) = CSF_VTBL(0x00832F18);
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00b4 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0047F4E0, TAtacarRafaga_Initialize, "auto");
