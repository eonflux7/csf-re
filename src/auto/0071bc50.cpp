#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange, 0x004109E0,
         (void* self, void* edx, void* source,
          std::uint32_t position, std::uint32_t count));

static std::uint32_t* __cdecl FillStringPairRecordRange(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* source)
{
    if (first != last)
    {
        void* secondSourceString =
            reinterpret_cast<std::uint8_t*>(source) + 0x1C;

        do
        {
            AssignStringRange(first, nullptr, source, 0, 0xFFFFFFFFu);
            AssignStringRange(
                reinterpret_cast<std::uint8_t*>(first) + 0x1C,
                nullptr, secondSourceString, 0, 0xFFFFFFFFu);

            CSF_AT(std::uint32_t, first, 0x38) =
                CSF_AT(std::uint32_t, source, 0x38);
            CSF_AT(std::uint32_t, first, 0x3C) =
                CSF_AT(std::uint32_t, source, 0x3C);

            first += 16;
        } while (first != last);
    }

    return last;
}

HOOK(0x0071BC50, FillStringPairRecordRange, "auto");
