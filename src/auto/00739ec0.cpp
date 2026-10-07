#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __stdcall, DestroyTreeNodes, 0x00739B80,
                (void** root));

static int __fastcall C_00867444_ClearAndReset(
    C_00867444* self, void* /*edx*/)
{
    DestroyTreeNodes(
        CSF_AT(void**, reinterpret_cast<void*>(self->field_002c), 0x04));

    uint32_t sentinelAddress = self->field_002c;
    CSF_AT(uint32_t, reinterpret_cast<void*>(sentinelAddress), 0x04) =
        sentinelAddress;

    sentinelAddress = self->field_002c;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(sentinelAddress), 0x00) =
        sentinelAddress;

    sentinelAddress = self->field_002c;
    CSF_AT(uint32_t, reinterpret_cast<void*>(sentinelAddress), 0x08) =
        sentinelAddress;

    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 1.0f;
    self->field_0058 = 1.0f;
    self->field_005c = 0;
    self->field_005d = 0;
    self->field_005e = 1;
    return 0;
}

HOOK(0x00739EC0, C_00867444_ClearAndReset, "auto");
