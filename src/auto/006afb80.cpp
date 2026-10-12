#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(const void*, __fastcall, Cheats_GrowEntryStorage, 0x00428D60, (void* self, void* , int additionalCapacity));

static uint32_t* __fastcall S_0041e5f0_InitializeOwnerHelper(
    S_0041e5f0* self, void* /*edx*/, uint32_t owner)
{
    // Retain the packet's additional address classification.
    (void)CSF_VTBL(0x0041E5F0);

    self->field_000c = 10;
    self->field_0004 = 0;
    self->field_0000 = 0;
    CSF_AT(uint32_t, self, 0x1C) = 10;
    self->field_0010 = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 1065353216;
    self->field_0020 = owner;

    uint32_t capacity = self->field_0004;
    self->field_0008 = 0;
    if (capacity < 0x40)
        S_0041e5f0_GrowArrayCapacity(self, nullptr, 64 - capacity);

    capacity = CSF_AT(uint32_t, self, 0x14);
    if (capacity < 0x40)
        Cheats_GrowEntryStorage(
            reinterpret_cast<uint8_t*>(self) + 0x10, nullptr, 64 - capacity);

    CSF_AT(uint32_t, self, 0x18) = 64;

    uint32_t entryIndex = 2;
    uint32_t byteOffset = 0;
    do
    {
        auto* entries = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(uint16_t, entries, byteOffset + 4) = 0;
        CSF_AT(uint32_t, entries, byteOffset) = 0xFFFFFFFF;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0010),
               byteOffset) = entryIndex - 2;

        entries = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(uint16_t, entries, byteOffset + 520) = 0;
        CSF_AT(uint32_t, entries, byteOffset + 516) = 0xFFFFFFFF;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0010),
               byteOffset + 516) = entryIndex - 1;

        entries = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(uint32_t, entries, byteOffset + 1032) = 0xFFFFFFFF;
        CSF_AT(uint16_t, entries, byteOffset + 1032 + 4) = 0;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0010),
               byteOffset + 1032) = entryIndex;

        entries = reinterpret_cast<uint8_t*>(self->field_0010);
        CSF_AT(uint32_t, entries, byteOffset + 1548) = 0xFFFFFFFF;
        CSF_AT(uint16_t, entries, byteOffset + 1548 + 4) = 0;
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0010),
               byteOffset + 1548) = entryIndex + 1;

        entryIndex += 4;
        byteOffset += 2064;
    }
    while (entryIndex - 2 < 0x40);

    return &self->field_0000;
}

HOOK(0x006AFB80, S_0041e5f0_InitializeOwnerHelper, "auto");
