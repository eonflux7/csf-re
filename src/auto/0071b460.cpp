#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source,
          std::uint32_t position, std::uint32_t count));

static void* __fastcall CopyConstructStringPairRecord(
    void* self, void* /*edx*/, const void* source)
{
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;
    AssignStringSubstring(self, nullptr, source, 0, 0xFFFFFFFFu);

    auto* secondString = static_cast<std::uint8_t*>(self) + 0x1C;
    const auto* sourceSecondString =
        static_cast<const std::uint8_t*>(source) + 0x1C;

    CSF_AT(std::uint32_t, secondString, 0x18) = 15;
    CSF_AT(std::uint32_t, secondString, 0x14) = 0;
    CSF_AT(std::uint8_t, secondString, 0x04) = 0;
    AssignStringSubstring(secondString, nullptr, sourceSecondString, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x38) =
        CSF_AT(std::uint32_t, source, 0x38);
    CSF_AT(std::uint32_t, self, 0x3C) =
        CSF_AT(std::uint32_t, source, 0x3C);

    return self;
}

HOOK(0x0071B460, CopyConstructStringPairRecord, "auto");
