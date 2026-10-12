#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static std::uint32_t* __fastcall TypeRegistry_CopyConstructEntry(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);

    auto* name = reinterpret_cast<std::uint8_t*>(self) + 0x0C;
    CSF_AT(std::uint32_t, name, 0x18) = 15;
    CSF_AT(std::uint32_t, name, 0x14) = 0;
    CSF_AT(std::uint8_t, name, 0x04) = 0;
    AssignStringSubstring(
        name, nullptr, reinterpret_cast<std::uint8_t*>(source) + 0x0C,
        0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    return self;
}

HOOK(0x0054D900, TypeRegistry_CopyConstructEntry, "auto");
