#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

static int __fastcall S_004f21d0_AssignFromRecord(
    S_004f21d0* self, void* /*edx*/, S_004f21d0* record)
{
    AssignStringRange_488830(self, nullptr, record, 0, 0xFFFFFFFF);

    self->field_001c = record->field_001c;
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, record, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, record, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, record, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, record, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, record, 0x30);
    self->field_0034 = record->field_0034;
    self->field_0038 = record->field_0038;
    self->field_003c = record->field_003c;
    CSF_AT(std::uint32_t, self, 0x40) = CSF_AT(std::uint32_t, record, 0x40);
    CSF_AT(std::uint32_t, self, 0x44) = CSF_AT(std::uint32_t, record, 0x44);
    CSF_AT(std::uint32_t, self, 0x48) = CSF_AT(std::uint32_t, record, 0x48);
    CSF_AT(std::uint32_t, self, 0x4C) = CSF_AT(std::uint32_t, record, 0x4C);

    auto* destinationBytes = reinterpret_cast<std::uint8_t*>(self);
    auto* sourceBytes = reinterpret_cast<std::uint8_t*>(record);
    AssignStringSubstring(destinationBytes + 0x50, nullptr, sourceBytes + 0x50, 0, -1);
    AssignStringSubstring(destinationBytes + 0x6C, nullptr, sourceBytes + 0x6C, 0, -1);
    AssignStringSubstring(destinationBytes + 0x88, nullptr, sourceBytes + 0x88, 0, -1);

    CSF_AT(std::uint32_t, self, 0xA4) = CSF_AT(std::uint32_t, record, 0xA4);
    CSF_AT(std::uint32_t, self, 0xA8) = CSF_AT(std::uint32_t, record, 0xA8);
    self->field_00ac = record->field_00ac;
    self->field_00b0 = record->field_00b0;
    self->field_00b4 = record->field_00b4;
    self->field_00b8 = record->field_00b8;
    self->field_00bc = record->field_00bc;
    self->field_00c0 = record->field_00c0;
    self->field_00c4 = record->field_00c4;

    return reinterpret_cast<int>(self);
}

HOOK(0x00512D80, S_004f21d0_AssignFromRecord, "auto");
