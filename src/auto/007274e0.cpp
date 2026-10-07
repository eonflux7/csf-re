#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl FillStringRecords(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* source)
{
    if (first != last)
    {
        const void* sourceString =
            reinterpret_cast<std::uint8_t*>(source) + 4;

        do
        {
            CSF_AT(std::uint32_t, first, 0x00) =
                CSF_AT(std::uint32_t, source, 0x00);

            AssignStringSubstring(
                reinterpret_cast<std::uint8_t*>(first) + 4,
                nullptr, sourceString, 0, 0xFFFFFFFFu);

            CSF_AT(std::uint32_t, first, 0x20) =
                CSF_AT(std::uint32_t, source, 0x20);

            first += 9;
        } while (first != last);
    }

    return last;
}

HOOK(0x007274E0, FillStringRecords, "auto");
