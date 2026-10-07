#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source,
          std::uint32_t start, std::uint32_t count));

static int __stdcall CopyConstructRecord(void* destination, const void* source)
{
    if (destination == nullptr)
        return 0;

    CSF_AT(std::uint32_t, destination, 0x00) =
        CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, destination, 0x1C) = 15;
    CSF_AT(std::uint32_t, destination, 0x18) = 0;
    CSF_AT(std::uint8_t, destination, 0x08) = 0;

    AssignStringSubstring(
        static_cast<std::uint8_t*>(destination) + 4,
        nullptr,
        static_cast<const std::uint8_t*>(source) + 4,
        0,
        0xFFFFFFFFu);

    const std::int32_t result = CSF_AT(std::int32_t, source, 0x20);
    CSF_AT(std::int32_t, destination, 0x20) = result;
    return result;
}

HOOK(0x00727410, CopyConstructRecord, "auto");
