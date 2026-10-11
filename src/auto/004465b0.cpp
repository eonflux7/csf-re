#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_00831230_Construct, 0x0044DDE0, (void* self, void* edx));

static uint32_t* __fastcall C_00831120_Construct(C_00831120* self, void* /*edx*/)
{
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x0082D418));
    self->vtbl = CSF_VTBL(0x00831120);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00831118));

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 10;

    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 10;

    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 10;

    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 10;

    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 10;

    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 10;

    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 10;

    self->field_0080 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 10;

    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 10;

    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 10;

    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 10;

    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 10;

    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00e4 = 10;

    self->field_00ec = 0;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 10;

    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0108 = 0;
    self->field_010c = 10;

    self->field_00e8 = 0;
    self->field_00fc = 0;
    self->field_0110 = 0;
    self->field_00c4 = 0;
    self->field_00c0 = 0;
    self->field_0008 = 0;
    self->field_003c = 0;

    void* childStorage = Mem_Alloc(44);
    if (childStorage)
        self->field_0114 = reinterpret_cast<uint32_t>(
            C_00831230_Construct(childStorage, nullptr));
    else
        self->field_0114 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004465B0, C_00831120_Construct, "auto");
