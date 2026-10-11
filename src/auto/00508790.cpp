#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Grows the record buffer when it is full (called with the requested capacity).
CSF_FUNC(int, __fastcall, sub_508240, 0x00508240, (void* self, void* edx, int a2));

// Assigns a string range into the record's string member (std::string-style assign).
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Vtable referenced by the original's call site to sub_508240 (kept as a file-scope binding).
[[maybe_unused]] static const auto s_S00508240Vtable = CSF_VTBL(0x00508240);

// Appends a 56-byte record copied from a2 to the buffer; returns a pointer to the new record.
static uint32_t* __fastcall S_00508240_AppendRecord(S_00508240* self, void* /*edx*/, uint32_t* a2, int a3)
{
    if (self->field_0008 >= self->field_0004)
    {
        int growRequest = a3;
        if (!a3)
            growRequest = self->field_000c;
        sub_508240(self, nullptr, growRequest);
    }

    // Re-read the count and buffer base after a possible grow.
    uint32_t index = self->field_0008;
    uint8_t* record = reinterpret_cast<uint8_t*>(self->field_0000) + index * 56u;
    self->field_0008 = index + 1;

    CSF_AT(uint32_t, record, 0x00) = a2[0];
    CSF_AT(uint32_t, record, 0x04) = a2[1];
    CSF_AT(uint32_t, record, 0x08) = a2[2];
    CSF_AT(uint32_t, record, 0x0C) = a2[3];
    CSF_AT(uint32_t, record, 0x10) = a2[4];
    CSF_AT(uint32_t, record, 0x14) = a2[5];
    CSF_AT(uint32_t, record, 0x18) = a2[6];

    AssignStringRange_488830(record + 0x1C, nullptr, reinterpret_cast<void*>(a2 + 7), 0, 0xFFFFFFFF);

    return reinterpret_cast<uint32_t*>(record);
}

HOOK(0x00508790, S_00508240_AppendRecord, "auto");
