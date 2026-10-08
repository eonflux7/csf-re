#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static std::uint32_t __fastcall C_008655e8_RemoveEntry(
    C_008655e8* self, void* /*edx*/, int value)
{
    const std::uint32_t initialBegin = self->field_001c;
    const std::uint32_t count = initialBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0020 - initialBegin) >> 2)
        : 0;

    if (count == 0)
        return count;

    const std::uint32_t begin = self->field_001c;
    std::uint32_t entryAddress = begin;
    std::uint32_t index = 0;

    while (*reinterpret_cast<const std::uint32_t*>(entryAddress) !=
           static_cast<std::uint32_t>(value))
    {
        ++index;
        entryAddress += 4;
        if (index >= count)
            return count;
    }

    const std::uint32_t end = self->field_0020;
    const std::uint32_t destination = begin + index * 4;
    const std::uint32_t source = destination + 4;
    const std::uint32_t byteCount =
        static_cast<std::uint32_t>(
            static_cast<std::int32_t>(end - source) >> 2) << 2;

    _memcpy(reinterpret_cast<void*>(destination),
               reinterpret_cast<const void*>(source), byteCount);

    const std::uint32_t newEnd = self->field_0020 - 4;
    self->field_0020 = newEnd;
    return newEnd;
}

HOOK(0x00727280, C_008655e8_RemoveEntry, "auto");
