#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static void* __fastcall TypeRegistry_CopyEntry(void* self, void* /*edx*/, const void* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);

    auto* name = static_cast<std::uint8_t*>(self) + 4;
    CSF_AT(std::uint32_t, name, 0x18) = 15;
    CSF_AT(std::uint32_t, name, 0x14) = 0;
    CSF_AT(std::uint8_t, name, 0x04) = 0;
    AssignStringSubstring(name, nullptr,
                          static_cast<const std::uint8_t*>(source) + 4,
                          0, static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);
    CSF_AT(std::uint8_t, self, 0x2C) = CSF_AT(std::uint8_t, source, 0x2C);
    CSF_AT(std::uint8_t, self, 0x2D) = CSF_AT(std::uint8_t, source, 0x2D);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    return self;
}

HOOK(0x0054D960, TypeRegistry_CopyEntry, "auto");
