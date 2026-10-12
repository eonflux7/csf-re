#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_54B180, 0x0054B180, ());

static C_008442a0* __fastcall C_008442a0_ConstructArchiveWrapper(
    C_008442a0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008442A0);
    self->field_0004 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;

    const int sentinelAddress = sub_54B180();
    CSF_AT(uint32_t, self, 0x9C) = static_cast<uint32_t>(sentinelAddress);
    CSF_AT(uint8_t, reinterpret_cast<void*>(sentinelAddress), 0x19) = 1;

    auto* sentinel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x9C));
    CSF_AT(uint32_t, sentinel, 0x04) = CSF_AT(uint32_t, self, 0x9C);
    sentinel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x9C));
    CSF_AT(uint32_t, sentinel, 0x00) = CSF_AT(uint32_t, self, 0x9C);
    sentinel = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x9C));
    CSF_AT(uint32_t, sentinel, 0x08) = CSF_AT(uint32_t, self, 0x9C);

    CSF_AT(uint32_t, self, 0xA0) = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_00a4 = 0;
    return self;
}

HOOK(0x0054C140, C_008442a0_ConstructArchiveWrapper, "auto");
