#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static int __fastcall C_0082d410_InitFromEntry(C_0082d410* self, void* /*edx*/, C_0082d410* source)
{
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0004) = 0;

    AssignStringSubstring(self, nullptr, source, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);

    return reinterpret_cast<int>(self);
}

HOOK(0x00433EA0, C_0082d410_InitFromEntry, "auto");
