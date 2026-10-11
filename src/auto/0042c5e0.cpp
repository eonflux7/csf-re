#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00830578_InitializeMenu, 0x0042B380, (void* self, void* edx));

static uint32_t* __fastcall C_00830578_Construct(
    C_00830578* self, void* /*edx*/, int menuArgument)
{
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0083042C));
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x008304D8);

    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00830578));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830528));
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x00830520);

    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f0 = 10;
    self->field_0014 = menuArgument;
    self->field_001c = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_0018 = 0;

    C_00830578_InitializeMenu(self, nullptr);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0042C5E0, C_00830578_Construct, "auto");
