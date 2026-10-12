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

// Resets the object's string-buffer state, copies the whole string from `source`,
// then copies the trailing scalar fields of the source record.
static int __fastcall C_0082d410_AssignFromSource(C_0082d410* self, void* /*edx*/, int a2)
{
    void* source = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a2));

    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;

    AssignStringSubstring(self, nullptr, source, 0u, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);

    CSF_AT(std::uint8_t, self, 0x30) = CSF_AT(std::uint8_t, source, 0x30);
    CSF_AT(std::uint8_t, self, 0x31) = CSF_AT(std::uint8_t, source, 0x31);
    CSF_AT(std::uint8_t, self, 0x32) = CSF_AT(std::uint8_t, source, 0x32);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x00774E90, C_0082d410_AssignFromSource, "auto");
