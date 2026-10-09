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
CSF_FUNC(uint32_t*, __fastcall, CopyEmbeddedRecord, 0x00776760,
         (void* self, void* edx, int a2));

// Copies two string members and one embedded record from `source` into `self`.
// Each string member is reset to an empty string with a 15-character capacity
// before the assignment. The original returns `this` in eax.
static C_0082d410* __fastcall C_0082d410_CopyFromSource(C_0082d410* self, void* /*edx*/, int a2)
{
    // First string member (at +0x00 / +0x04 / +0x14 / +0x18).
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0004) = 0;
    AssignStringSubstring(self, nullptr, reinterpret_cast<const void*>(a2), 0, static_cast<std::uint32_t>(-1));

    // Second string member, embedded at +0x1C.
    CSF_AT(std::uint32_t, self, 0x34) = 15;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint8_t, self, 0x20) = 0;
    AssignStringSubstring(reinterpret_cast<std::uint8_t*>(self) + 0x1C, nullptr,
                          reinterpret_cast<const std::uint8_t*>(a2) + 0x1C, 0,
                          static_cast<std::uint32_t>(-1));

    // Embedded record at +0x38.
    CopyEmbeddedRecord(reinterpret_cast<std::uint8_t*>(self) + 0x38, nullptr,
                       a2 + 0x38);

    return self;
}

HOOK(0x007780A0, C_0082d410_CopyFromSource, "auto");
