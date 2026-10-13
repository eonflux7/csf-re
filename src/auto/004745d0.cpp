#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00831658_CopyStateTo(
    C_00831658* self, void* /*edx*/, int destinationAddress)
{
    auto* destination = reinterpret_cast<C_00831658*>(destinationAddress);

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
    destination->field_0050 = self->field_0050;
    CSF_AT(uint32_t, destination, 0x54) = CSF_AT(uint32_t, self, 0x54);
    CSF_AT(uint32_t, destination, 0x58) = CSF_AT(uint32_t, self, 0x58);
    CSF_AT(uint32_t, destination, 0x5C) = CSF_AT(uint32_t, self, 0x5C);

    return reinterpret_cast<int>(
        reinterpret_cast<uint8_t*>(destination) + 0x54);
}

HOOK(0x004745D0, C_00831658_CopyStateTo, "auto");
