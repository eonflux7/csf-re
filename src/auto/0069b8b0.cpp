#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: AssignStringSubstring (__thiscall, 3 stack args, ret 12)
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a run of 32-byte entries: each entry holds a DWORD at +0 and a string object at +4.
// Returns the destination position just past the last copied entry (or the destination itself if the range is empty).
static int __cdecl CopyStringEntryRange(std::uint32_t* first, std::uint32_t* last, std::uint8_t* dest)
{
    if (first == last) {
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(dest));
    }

    std::uint32_t* source = first;
    std::uint8_t* out = dest;
    do {
        reinterpret_cast<std::uint32_t*>(out)[0] = source[0];
        AssignStringSubstring(out + 4, nullptr, source + 1, 0u, 0xFFFFFFFFu);
        source += 8;
        out += 32;
    } while (source != last);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(out));
}

HOOK(0x0069B8B0, CopyStringEntryRange, "auto");
