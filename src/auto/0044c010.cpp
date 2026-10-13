#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static void* __fastcall CopyConstructStringRecord(
    void* self, void* /*edx*/, const void* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x0082D448);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, source, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = CSF_AT(std::uint32_t, source, 0x18);
    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint8_t, self, 0x28) = CSF_AT(std::uint8_t, source, 0x28);
    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, source, 0x2C);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);

    void* destinationString = static_cast<std::uint8_t*>(self) + 0x40;
    const void* sourceString = static_cast<const std::uint8_t*>(source) + 0x40;
    CSF_AT(std::uint32_t, destinationString, 0x18) = 15;
    CSF_AT(std::uint32_t, destinationString, 0x14) = 0;
    CSF_AT(std::uint8_t, destinationString, 0x04) = 0;
    AssignStringSubstring(
        destinationString, nullptr, sourceString, 0,
        static_cast<std::uint32_t>(-1));

    CSF_AT(std::uint32_t, self, 0x5C) = CSF_AT(std::uint32_t, source, 0x5C);
    CSF_AT(std::uint32_t, self, 0x60) = CSF_AT(std::uint32_t, source, 0x60);
    return self;
}

HOOK(0x0044C010, CopyConstructStringRecord, "auto");
