#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_0082daf4_ResetEntries(C_0082daf4* self, void* /*edx*/)
{
    auto* entry = reinterpret_cast<uint8_t*>(self->field_0010);
    CSF_AT(uint32_t, entry, 0x00) = 0;
    float initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x04);
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 28;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x20);
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 56;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x3C);
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 84;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x58);
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 112;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x74);
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 140;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0x90);
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 168;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0xAC);
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(float, entry, 0x08) = initialValue;

    entry = reinterpret_cast<uint8_t*>(self->field_0010) + 196;
    CSF_AT(uint32_t, entry, 0x00) = 0;
    initialValue = CSF_AT(float, reinterpret_cast<uint8_t*>(self->field_0010), 0xC8);
    CSF_AT(uint32_t, entry, 0x14) = 0;
    CSF_AT(float, entry, 0x04) = initialValue;
    CSF_AT(float, entry, 0x08) = initialValue;

    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x0040B5B0, C_0082daf4_ResetEntries, "auto");
