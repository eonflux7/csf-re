#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a perf-counter record from source (stack arg) into this.
// Layout: +0 dword, +4 string, +32 string, +60 dword, +64..+68 bytes,
// +72 dword, +76 block of 0x38 bytes.
static char* __fastcall NetPerfCounters_CopyRecord(char* self, void* /*edx*/, const char* source)
{
    CSF_AT(std::uint32_t, self, 0) = CSF_AT(std::uint32_t, source, 0);

    AssignStringSubstring(self + 4, nullptr, source + 4, 0, static_cast<std::uint32_t>(-1));
    AssignStringSubstring(self + 32, nullptr, source + 32, 0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);

    CSF_AT(std::uint8_t, self, 0x40) = CSF_AT(std::uint8_t, source, 0x40);
    CSF_AT(std::uint8_t, self, 0x41) = CSF_AT(std::uint8_t, source, 0x41);
    CSF_AT(std::uint8_t, self, 0x42) = CSF_AT(std::uint8_t, source, 0x42);
    CSF_AT(std::uint8_t, self, 0x43) = CSF_AT(std::uint8_t, source, 0x43);
    CSF_AT(std::uint8_t, self, 0x44) = CSF_AT(std::uint8_t, source, 0x44);

    CSF_AT(std::uint32_t, self, 0x48) = CSF_AT(std::uint32_t, source, 0x48);

    std::memcpy(self + 0x4C, source + 0x4C, 0x38);

    return self;
}

HOOK(0x78BDA0, NetPerfCounters_CopyRecord, "auto");
