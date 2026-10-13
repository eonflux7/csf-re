#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bound by address (fastcall-style binding as given in the packet).
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies a 0x3F-byte record from `source` into `self`, then returns `self`.
// Layout (from the original): two dwords at +0/+4, a string sub-object at +8
// (assigned via AssignStringRange), and dword/byte/word fields at +0x24..+0x3E.
static char* __fastcall CopyRecordFromSource(char* self, void* /*edx*/, char* source)
{
    // Two leading dwords.
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);

    // String sub-object at self+8: reset its header fields, then assign the range.
    char* stringObject = self + 0x08;
    CSF_AT(std::uint32_t, stringObject, 0x18) = 7;
    CSF_AT(std::uint32_t, stringObject, 0x14) = 0;
    CSF_AT(std::uint16_t, stringObject, 0x04) = 0;
    AssignStringRange_488830(stringObject, nullptr, source + 0x08, 0, 0xFFFFFFFFu);

    // Trailing fields.
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint8_t,  self, 0x28) = CSF_AT(std::uint8_t,  source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint16_t, self, 0x3C) = CSF_AT(std::uint16_t, source, 0x3C);
    CSF_AT(std::uint8_t,  self, 0x3E) = CSF_AT(std::uint8_t,  source, 0x3E);

    return self;
}

HOOK(0x00749060, CopyRecordFromSource, "auto");
