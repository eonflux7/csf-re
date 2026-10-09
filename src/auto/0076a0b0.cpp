#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// AssignStringSubstring: __thiscall, 3 stack args, ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a range of 68-byte input binding records [sourceBegin, sourceEnd) into
// destination storage. Returns the destination cursor just past the last copied record,
// or the destination start unchanged when the range is empty.
static int __cdecl InputBindings_CopyBindingRange(int sourceBegin, int sourceEnd, int destinationBegin)
{
    if (sourceBegin == sourceEnd)
        return destinationBegin;

    std::uint8_t* source = reinterpret_cast<std::uint8_t*>(sourceBegin);
    std::uint8_t* destination = reinterpret_cast<std::uint8_t*>(destinationBegin);
    const std::uint8_t* end = reinterpret_cast<const std::uint8_t*>(sourceEnd);

    do
    {
        CSF_AT(std::uint32_t, destination, 0) = CSF_AT(std::uint32_t, source, 0);
        CSF_AT(std::uint32_t, destination, 4) = CSF_AT(std::uint32_t, source, 4);
        CSF_AT(std::uint8_t, destination, 8) = CSF_AT(std::uint8_t, source, 8);
        CSF_AT(std::uint16_t, destination, 9) = CSF_AT(std::uint16_t, source, 9);
        CSF_AT(std::uint32_t, destination, 12) = CSF_AT(std::uint32_t, source, 12);
        CSF_AT(std::uint32_t, destination, 16) = CSF_AT(std::uint32_t, source, 16);
        CSF_AT(std::uint8_t, destination, 20) = CSF_AT(std::uint8_t, source, 20);

        AssignStringSubstring(destination + 24, nullptr, source + 24, 0, 0xFFFFFFFFu);

        CSF_AT(std::uint32_t, destination, 52) = CSF_AT(std::uint32_t, source, 52);
        CSF_AT(std::uint32_t, destination, 56) = CSF_AT(std::uint32_t, source, 56);
        CSF_AT(std::uint32_t, destination, 60) = CSF_AT(std::uint32_t, source, 60);
        CSF_AT(std::uint32_t, destination, 64) = CSF_AT(std::uint32_t, source, 64);

        source += 68;
        destination += 68;
    } while (source != end);

    return reinterpret_cast<int>(destination);
}

HOOK(0x0076A0B0, InputBindings_CopyBindingRange, "auto");
