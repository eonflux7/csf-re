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

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, sub_772E00, 0x00772E00, (void* self, void* edx, void* a4a));

// Copies one animation definition record from `source` into `self`.
// Layout: +0x00 id, +0x04 name string, +0x20 sub-record, +0x30/+0x34 ints,
// +0x38 string, +0x54 int, +0x58..+0x5D flag bytes.
static char* __fastcall AnimationDefs_CopyEntry(char* self, void* /*edx*/, std::uint8_t* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);

    AssignStringSubstring(self + 0x04, nullptr, source + 0x04, 0, static_cast<std::uint32_t>(-1));

    sub_772E00(self + 0x20, nullptr, source + 0x20);

    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);

    AssignStringSubstring(self + 0x38, nullptr, source + 0x38, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x54) = CSF_AT(std::uint32_t, source, 0x54);

    CSF_AT(std::uint8_t, self, 0x58) = CSF_AT(std::uint8_t, source, 0x58);
    CSF_AT(std::uint8_t, self, 0x59) = CSF_AT(std::uint8_t, source, 0x59);
    CSF_AT(std::uint8_t, self, 0x5A) = CSF_AT(std::uint8_t, source, 0x5A);
    CSF_AT(std::uint8_t, self, 0x5B) = CSF_AT(std::uint8_t, source, 0x5B);
    CSF_AT(std::uint8_t, self, 0x5C) = CSF_AT(std::uint8_t, source, 0x5C);
    CSF_AT(std::uint8_t, self, 0x5D) = CSF_AT(std::uint8_t, source, 0x5D);

    return self;
}

HOOK(0x773200, AnimationDefs_CopyEntry, "auto");
