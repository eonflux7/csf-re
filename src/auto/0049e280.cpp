#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00834050_Construct, 0x0049D280, (void* self, void* edx, int a2, int a3));

static uint32_t* __fastcall C_00834198_Construct(
    C_00834198* self, void* /*edx*/, int a2, int a3)
{
    C_00834050_Construct(self, nullptr, a2, a3);

    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00834198));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834118));
    CSF_AT(uint32_t, self, 0x008) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834114));
    self->field_000c = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834108));
    CSF_AT(uint32_t, self, 0x010) =
        reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00834100));
    self->field_0014 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x008340FC));

    self->field_01c8 = 0;
    self->field_01cc = 0;
    self->field_01d0 = 0;
    self->field_01d4 = 0;
    self->field_01d8 = 0;
    self->field_01dc = 0;
    self->field_01ec = 0;
    self->field_01f0 = 0;
    self->field_01f4 = 0;
    self->field_01e0 = 0;
    self->field_01e4 = 0;
    self->field_01e8 = 0;
    self->field_01f0 = 0;
    self->field_01f4 = 0;
    self->field_01ec = 1065353216;
    self->field_0208 = 1;
    self->field_0218 = 0;
    self->field_021c = 0;
    self->field_0220 = 0;
    self->field_01c0 = 0;
    self->field_01c4 = 0;
    self->field_020c = 0;
    self->field_0210 = 0;
    self->field_0214 = 0;
    self->field_0215 = 0;
    self->field_01b8 = 0;
    self->field_01b9 = 0;
    self->field_01ba = 0;
    self->field_01bc = 0;
    self->field_0224 = 0;
    self->field_0225 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0049E280, C_00834198_Construct, "auto");
