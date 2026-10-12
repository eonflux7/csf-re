#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies an 8-dword header from the source buffer into the record, resets the
// record's string-capacity fields and byte flag, then assigns the string that
// follows the header (source offset 0x20) into the record's string member at +0x20.
static char* __fastcall CopyRecordFromBufferAndAssignName(char* self, void* /*edx*/, const std::uint32_t* a2)
{
    CSF_AT(std::uint32_t, self, 0x00) = a2[0];
    CSF_AT(std::uint32_t, self, 0x04) = a2[1];
    CSF_AT(std::uint32_t, self, 0x08) = a2[2];
    CSF_AT(std::uint32_t, self, 0x0C) = a2[3];
    CSF_AT(std::uint32_t, self, 0x10) = a2[4];
    CSF_AT(std::uint32_t, self, 0x14) = a2[5];
    CSF_AT(std::uint32_t, self, 0x18) = a2[6];
    CSF_AT(std::uint32_t, self, 0x1C) = a2[7];

    CSF_AT(std::uint32_t, self, 0x38) = 15;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint8_t, self, 0x24) = 0;

    AssignStringSubstring(self + 0x20, nullptr, a2 + 8, 0u, 0xFFFFFFFFu);
    return self;
}

HOOK(0x774E20, CopyRecordFromBufferAndAssignName, "auto");
