#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies the string held in `source` into every entry of the range [first, last),
// stride 32 bytes, then propagates the source's 32-bit tag at +0x1C into each entry.
// Returns the last tag value read (0 when the range is empty).
static uint32_t __cdecl AssignSourceToEntryRange(C_0082d410* first, C_0082d410* last, C_0082d410* source)
{
    uint32_t lastTag = 0;

    for (C_0082d410* entry = first; entry != last;
         entry = reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(entry) + 32))
    {
        AssignStringSubstring(entry, nullptr, source, 0, static_cast<std::uint32_t>(-1));
        lastTag = CSF_AT(uint32_t, source, 0x1C);
        CSF_AT(uint32_t, entry, 0x1C) = lastTag;
    }
    return lastTag;
}

HOOK(0x0046D650, AssignSourceToEntryRange, "auto");
