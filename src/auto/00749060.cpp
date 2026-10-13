#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Bound by address: the original calls this with (this+8, source+8, 0, 0xFFFFFFFF).
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies a fixed-layout record from source into self (__thiscall, one stack arg, ret 4).
// Layout: two dwords at +0/+4, an embedded string-like sub-object at +8 (assigned from source+8),
// then dword/byte/word fields at +0x24..+0x3E.
static char* __fastcall CopyRecordFromSource(char* self, void* /*edx*/, char* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);

    char* subObject = self + 0x08;
    CSF_AT(std::uint32_t, subObject, 0x10) = 7;   // +0x18 of self
    CSF_AT(std::uint32_t, subObject, 0x0C) = 0;   // +0x14 of self
    CSF_AT(std::uint16_t, subObject, 0x00) = 0;   // +0x08 word write
    AssignStringRange_488830(subObject, nullptr, source + 0x08, 0, 0xFFFFFFFF);

    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint8_t, self, 0x28) = CSF_AT(std::uint8_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint16_t, self, 0x3C) = CSF_AT(std::uint16_t, source, 0x3C);
    CSF_AT(std::uint8_t, self, 0x3E) = CSF_AT(std::uint8_t, source, 0x3E);

    return self;
}

HOOK(0x00749060, CopyRecordFromSource, "auto");
