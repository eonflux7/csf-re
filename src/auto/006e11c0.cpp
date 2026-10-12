#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static int __fastcall C_0082d410_InitFrom(C_0082d410* self, void* /*edx*/, C_0082d410* a2)
{
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;

    AssignStringSubstring(self, nullptr, a2, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, a2, 0x1C);

    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, a2, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, a2, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, a2, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, a2, 0x2C);

    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, a2, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, a2, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, a2, 0x38);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x006E11C0, C_0082d410_InitFrom, "auto");
