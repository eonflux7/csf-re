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
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Copies the 0x38..0x78 block of the source entry into the destination entry.
// Each field is a 4-byte value except the byte at 0x74.
static void HudStringShow_CopyEntryFields(std::uint8_t* dst, const std::uint8_t* src) {
    CSF_AT(std::uint32_t, dst, 0x38) = CSF_AT(std::uint32_t, src, 0x38);
    CSF_AT(std::uint32_t, dst, 0x3C) = CSF_AT(std::uint32_t, src, 0x3C);
    CSF_AT(std::uint32_t, dst, 0x40) = CSF_AT(std::uint32_t, src, 0x40);
    CSF_AT(std::uint32_t, dst, 0x44) = CSF_AT(std::uint32_t, src, 0x44);
    CSF_AT(std::uint32_t, dst, 0x48) = CSF_AT(std::uint32_t, src, 0x48);
    CSF_AT(std::uint32_t, dst, 0x4C) = CSF_AT(std::uint32_t, src, 0x4C);
    CSF_AT(std::uint32_t, dst, 0x50) = CSF_AT(std::uint32_t, src, 0x50);
    CSF_AT(std::uint32_t, dst, 0x54) = CSF_AT(std::uint32_t, src, 0x54);
    CSF_AT(std::uint32_t, dst, 0x58) = CSF_AT(std::uint32_t, src, 0x58);
    CSF_AT(std::uint32_t, dst, 0x5C) = CSF_AT(std::uint32_t, src, 0x5C);
    CSF_AT(std::uint32_t, dst, 0x60) = CSF_AT(std::uint32_t, src, 0x60);
    CSF_AT(std::uint32_t, dst, 0x64) = CSF_AT(std::uint32_t, src, 0x64);
    CSF_AT(std::uint32_t, dst, 0x68) = CSF_AT(std::uint32_t, src, 0x68);
    CSF_AT(std::uint32_t, dst, 0x6C) = CSF_AT(std::uint32_t, src, 0x6C);
    CSF_AT(std::uint32_t, dst, 0x70) = CSF_AT(std::uint32_t, src, 0x70);
    CSF_AT(std::uint8_t, dst, 0x74) = CSF_AT(std::uint8_t, src, 0x74);
    CSF_AT(std::uint32_t, dst, 0x78) = CSF_AT(std::uint32_t, src, 0x78);
}

// Assigns the source string entry (a2) into this entry: both string members
// are replaced with the full source text, then the scalar fields are copied.
static int __fastcall HudStringShow_AssignEntry(C_0082d410* self, void* /*edx*/, int sourceEntry) {
    auto* dst = reinterpret_cast<std::uint8_t*>(self);
    auto* src = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(static_cast<std::uint32_t>(sourceEntry)));

    AssignStringSubstring(self, nullptr, src, 0, 0xFFFFFFFF);
    AssignStringRange_488830(dst + 0x1C, nullptr, src + 0x1C, 0, 0xFFFFFFFF);

    HudStringShow_CopyEntryFields(dst, src);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x004F2AD0, HudStringShow_AssignEntry, "auto");
