#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Binds to the game's string-assign routine (__thiscall, 3 stack args, ret 12).
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the string held by `source` into every 36-byte element in [rangeBegin, rangeEnd),
// then copies the element's trailing int (+0x1C) and word (+0x20) fields from `source`.
// Returns the +0x1C value read from `source` on the last iteration (0 if the range is empty).
static int __cdecl AssignStringAndCopyFieldsToRange(int rangeBegin, int rangeEnd, int source)
{
    int result = 0;
    int element = rangeBegin;

    while (element != rangeEnd)
    {
        AssignStringSubstring(reinterpret_cast<void*>(element), nullptr,
                              reinterpret_cast<const void*>(source), 0u, 0xFFFFFFFFu);

        result = CSF_AT(int, source, 0x1C);
        CSF_AT(int, element, 0x1C) = result;
        CSF_AT(std::uint16_t, element, 0x20) = CSF_AT(std::uint16_t, source, 0x20);

        element += 0x24;
    }

    return result;
}

HOOK(0x6C9150, AssignStringAndCopyFieldsToRange, "auto");
