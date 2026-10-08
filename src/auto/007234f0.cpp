#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __fastcall CopyConstructStringRecord(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x10);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, source, 0x14);

    auto* destinationString = reinterpret_cast<std::uint8_t*>(self) + 0x18;
    CSF_AT(std::uint32_t, destinationString, 0x14) = 0;
    CSF_AT(std::uint32_t, destinationString, 0x18) = 7;
    CSF_AT(std::uint16_t, destinationString, 0x04) = 0;

    AssignStringRange_488830(
        destinationString, nullptr,
        reinterpret_cast<std::uint8_t*>(source) + 0x18,
        0, UINT32_MAX);

    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);
    return self;
}

HOOK(0x007234F0, CopyConstructStringRecord, "auto");
