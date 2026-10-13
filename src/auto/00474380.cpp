#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082e0f4_CopyStateTo(
    C_0082e0f4* self, void* /*edx*/, int destinationAddress)
{
    auto* destination = reinterpret_cast<C_0082e0f4*>(destinationAddress);

    destination->field_0004 = self->field_0004;
    destination->field_000c = self->field_000c;
    destination->field_0010 = self->field_0010;
    destination->field_0018 = self->field_0018;
    destination->field_001c = self->field_001c;
    destination->field_0024 = self->field_0024;
    destination->field_002c = self->field_002c;
    destination->field_0020 = self->field_0020;
    destination->field_0030 = self->field_0030;
    destination->field_0028 = self->field_0028;
    destination->field_0034 = self->field_0034;
    CSF_AT(uint32_t, destination, 0x38) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint32_t, destination, 0x3C) = CSF_AT(uint32_t, self, 0x3C);
    destination->field_0040 = self->field_0040;
    CSF_AT(uint32_t, destination, 0x44) = CSF_AT(uint32_t, self, 0x44);
    CSF_AT(uint32_t, destination, 0x48) = CSF_AT(uint32_t, self, 0x48);
    destination->field_004c = self->field_004c;
    CSF_AT(uint32_t, destination, 0x50) = CSF_AT(uint32_t, self, 0x50);
    CSF_AT(uint32_t, destination, 0x54) = CSF_AT(uint32_t, self, 0x54);
    destination->field_0058 = self->field_0058;
    destination->field_005c = self->field_005c;
    destination->field_0060 = self->field_0060;
    destination->field_0064 = self->field_0064;
    destination->field_0068 = self->field_0068;
    destination->field_006c = self->field_006c;
    destination->field_0070 = self->field_0070;
    destination->field_0071 = self->field_0071;

    return destinationAddress;
}

HOOK(0x00474380, C_0082e0f4_CopyStateTo, "auto");
