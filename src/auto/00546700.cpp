#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __fastcall MenuExtras_CopyConstructEntry(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    auto* destinationBytes = reinterpret_cast<std::uint8_t*>(self);
    auto* sourceBytes = reinterpret_cast<std::uint8_t*>(source);

    CSF_AT(std::uint32_t, self, 0x00) =
        CSF_AT(std::uint32_t, source, 0x00);

    CSF_AT(std::uint32_t, self, 0x1C) = 15;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint8_t, self, 0x08) = 0;
    MenuExtras_AssignStringRange(
        destinationBytes + 0x04, nullptr, sourceBytes + 0x04,
        0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x20) =
        CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) =
        CSF_AT(std::uint32_t, source, 0x24);

    CSF_AT(std::uint32_t, self, 0x40) = 15;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint8_t, self, 0x2C) = 0;
    MenuExtras_AssignStringRange(
        destinationBytes + 0x28, nullptr, sourceBytes + 0x28,
        0, 0xFFFFFFFFu);

    return self;
}

HOOK(0x00546700, MenuExtras_CopyConstructEntry, "auto");
