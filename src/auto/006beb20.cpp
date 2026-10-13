#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(uint32_t*, __cdecl, GfxEnvironment_GetNamedResource, 0x006EE840, (const char* name));

static int __fastcall C_008605d8_Initialize(C_008605d8* self, void* /*edx*/)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x008605D8);

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;
    self->field_0028 = 10;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0038 = 10;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;

    CSF_AT(uint32_t, self, 0x048) = 10;
    CSF_AT(uint32_t, self, 0x03C) = 0;
    CSF_AT(uint32_t, self, 0x040) = 0;
    CSF_AT(uint32_t, self, 0x044) = 0;
    CSF_AT(uint32_t, self, 0x058) = 10;
    CSF_AT(uint32_t, self, 0x04C) = 0;
    CSF_AT(uint32_t, self, 0x050) = 0;
    CSF_AT(uint32_t, self, 0x054) = 0;

    self->field_0068 = 10;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0078 = 10;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0088 = 10;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_008c = 0;
    self->field_00a0 = 10;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;

    uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 0x0C4;
    for (int remaining = 15; remaining != 0; --remaining)
    {
        CSF_AT(uint32_t, entry, 0x0) = 0;
        CSF_AT(uint32_t, entry, 0x4) = 0;
        CSF_AT(uint8_t, entry, 0x8) = 0;
        entry += 12;
    }

    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_0184 = 0;
    self->field_0188 = 0;
    self->field_018c = 0;
    self->field_0004 = 0;
    self->field_0090 = 0;
    self->field_0091 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_0092 = 0;
    self->field_0008 = 0;

    self->field_0178 = reinterpret_cast<uint32_t>(
        GfxEnvironment_GetNamedResource(nullptr));
    self->field_017c = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x006BEB20, C_008605d8_Initialize, "auto");
