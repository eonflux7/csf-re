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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies one mission-script record (header fields, embedded string, trailing dwords)
// from `source` into `self`. Returns `self`, as the original does in eax.
static char* __fastcall MissionScript_CopyRecordFromSource(char* self, void* /*edx*/, char* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint8_t, self, 0x08) = CSF_AT(std::uint8_t, source, 0x08);
    CSF_AT(std::uint16_t, self, 0x09) = CSF_AT(std::uint16_t, source, 0x09);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x10);
    CSF_AT(std::uint8_t, self, 0x14) = CSF_AT(std::uint8_t, source, 0x14);

    // Embedded string object at self+0x18: capacity at +0x30, size at +0x2C, first byte at +0x1C.
    char* embeddedString = self + 0x18;
    CSF_AT(std::uint32_t, embeddedString, 0x18) = 15;
    CSF_AT(std::uint32_t, embeddedString, 0x14) = 0;
    CSF_AT(std::uint8_t, embeddedString, 0x04) = 0;
    AssignStringSubstring(embeddedString, nullptr, source + 0x18, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);
    CSF_AT(std::uint32_t, self, 0x40) = CSF_AT(std::uint32_t, source, 0x40);

    return self;
}

HOOK(0x00469FD0, MissionScript_CopyRecordFromSource, "auto");
