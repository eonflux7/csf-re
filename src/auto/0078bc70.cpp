#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a perf-counter record from `source` into `self`: two string members, a
// dword, five bytes, a dword and a 0x38-byte tail block. Returns `self`.
static char* __fastcall CopyPerfCounterRecord(char* self, void* /*edx*/, const char* source)
{
    std::uint8_t* dst = reinterpret_cast<std::uint8_t*>(self);
    const std::uint8_t* src = reinterpret_cast<const std::uint8_t*>(source);

    CSF_AT(std::uint32_t, dst, 0x00) = CSF_AT(std::uint32_t, src, 0x00);

    // First string member at dst+0x04 (object at ecx = dst+4).
    CSF_AT(std::uint32_t, dst, 0x1C) = 15;
    CSF_AT(std::uint32_t, dst, 0x18) = 0;
    CSF_AT(std::uint8_t, dst, 0x08) = 0;
    AssignStringSubstring(dst + 0x04, nullptr, src + 0x04, 0u, 0xFFFFFFFFu);

    // Second string member at dst+0x20 (object at ecx = dst+0x20).
    CSF_AT(std::uint32_t, dst, 0x38) = 15;
    CSF_AT(std::uint32_t, dst, 0x34) = 0;
    CSF_AT(std::uint8_t, dst, 0x24) = 0;
    AssignStringSubstring(dst + 0x20, nullptr, src + 0x20, 0u, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, dst, 0x3C) = CSF_AT(std::uint32_t, src, 0x3C);
    CSF_AT(std::uint8_t, dst, 0x40) = CSF_AT(std::uint8_t, src, 0x40);
    CSF_AT(std::uint8_t, dst, 0x41) = CSF_AT(std::uint8_t, src, 0x41);
    CSF_AT(std::uint8_t, dst, 0x42) = CSF_AT(std::uint8_t, src, 0x42);
    CSF_AT(std::uint8_t, dst, 0x43) = CSF_AT(std::uint8_t, src, 0x43);
    CSF_AT(std::uint8_t, dst, 0x44) = CSF_AT(std::uint8_t, src, 0x44);
    CSF_AT(std::uint32_t, dst, 0x48) = CSF_AT(std::uint32_t, src, 0x48);

    std::memcpy(dst + 0x4C, src + 0x4C, 0x38);

    return self;
}

HOOK(0x0078BC70, CopyPerfCounterRecord, "auto");
