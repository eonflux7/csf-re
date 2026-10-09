#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_503AA0, 0x00503AA0, (void* self, void* edx));

static CSceneLoader* __fastcall CSceneLoader_Construct(CSceneLoader* self, void* /*edx*/)
{
    sub_503AA0(self, nullptr);

    CSF_AT(uint32_t, self, 0x114) = 0;
    CSF_AT(uint32_t, self, 0x110) = 0;
    CSF_AT(uint8_t, self, 0xB5) = 0;
    CSF_AT(uint8_t, self, 0x118) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;

    self->vtbl = CSF_VTBL(0x0083F5D8);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083F5D0);

    return self;
}

HOOK(0x505BD0, CSceneLoader_Construct, "auto");
