#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Assigns the string of each 0x30-byte record in [first, last) into the
// destination record at the same stride, then copies the five dwords that
// sit at the record's +0x20 base (index -1 through +3) from source to
// destination. Returns the destination cursor after the last record.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static C_0082d410* __cdecl HudWidgets_CopyRecordRange(int first, int last, C_0082d410* dest)
{
    int cursor = first;

    if (first != last)
    {
        do
        {
            AssignStringSubstring(dest, nullptr, reinterpret_cast<const void*>(cursor), 0u, 0xFFFFFFFFu);

            // Record payload is addressed from the +0x20 base, matching the
            // original's esi/edi pointers (index -1 is the dword just below it).
            std::uint32_t* destWords = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(dest) + 0x20);
            std::uint32_t* srcWords = reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(cursor) + 0x20);

            destWords[-1] = srcWords[-1];
            destWords[0] = srcWords[0];
            destWords[1] = srcWords[1];
            destWords[2] = srcWords[2];
            destWords[3] = srcWords[3];

            cursor += 0x30;
            dest = reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(dest) + 0x30);
        } while (cursor != last);
    }

    return dest;
}

HOOK(0x00433FE0, HudWidgets_CopyRecordRange, "auto");
