// Reimplementation of 0x007835C0: WeaponDefs record copy constructor.
// Copies the leading dword, resets the embedded string at +4 (length 0, capacity 15,
// empty), appends the source string at +4, then copies the trailing six dwords at
// +0x20..+0x34 from the source record.

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

// Bound as in the packet: AssignStringSubstring(self, source, position, count), ret 12.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static std::uint32_t* __fastcall WeaponDefs_CopyRecord(std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    // Leading dword is copied directly.
    *self = *source;

    // Embedded string member at +4: reset to empty, capacity 15.
    std::uint32_t* nameWords = self + 1;
    nameWords[6] = 15;
    nameWords[5] = 0;
    CSF_AT(std::uint8_t, nameWords, 4) = 0;

    // Assign the source string (source + 4) into the member.
    AssignStringSubstring(nameWords, nullptr, source + 1, 0, 0xFFFFFFFFu);

    // Trailing numeric fields.
    self[8]  = source[8];
    self[9]  = source[9];
    self[10] = source[10];
    self[11] = source[11];
    self[12] = source[12];
    self[13] = source[13];

    return self;
}

HOOK(0x007835C0, WeaponDefs_CopyRecord, "auto");
