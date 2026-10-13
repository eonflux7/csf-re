#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMenuLoading_ConfigureLoader, 0x00538540, (CMenuLoading* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0085e7d0_Construct, 0x0069AE70, (C_0085e7d0* self, void*));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static int __fastcall CMenuLoading_Construct(CMenuLoading* self, void* /*edx*/, int loaderContext)
{
    self->vtbl = CSF_VTBL(0x0083042C);
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    self->vtbl = CSF_VTBL(0x008304E0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830488));
    C_0085e7d0_Construct(
        reinterpret_cast<C_0085e7d0*>(reinterpret_cast<uint8_t*>(self) + 0x10),
        nullptr);

    self->vtbl = CSF_VTBL(0x00842150);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00842100));
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x008420F8);

    self->field_0058 = 0;
    self->field_0059 = 0;
    self->field_005a = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(uint32_t, self, 0x80) = 0;
    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xA8) = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint32_t, self, 0xB8) = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 0;
    CSF_AT(uint32_t, self, 0xFC) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint32_t, self, 0x108) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint32_t, self, 0x110) = 0;
    CSF_AT(uint32_t, self, 0x114) = 0;
    CSF_AT(uint32_t, self, 0x118) = 0;

    self->field_0038 = loaderContext;
    self->m_ownedObjectA = 0;
    self->field_0040 = 0;
    self->field_0028 = 0;
    self->field_0044 = 0.0;
    self->field_0048 = 100.0;
    self->m_ownedBlock = 0;
    self->field_0054 = 0;
    self->field_0051 = 0;
    self->field_0030 = 0.0;
    self->field_0034 = 0.0;
    self->field_0050 = 0;

    CMenuLoading_ConfigureLoader(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x00538BE0, CMenuLoading_Construct, "auto");
