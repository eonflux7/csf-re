#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies 60-byte entries backward from [a2 - 60] down to a1 into the block ending at a3.
// Each entry: a dword at +0, three dwords at +4..+12, a 4-dword header at +12..+28 region
// and a string at +16 (assigned via AssignStringSubstring with the whole source string).
// Returns the new destination cursor.
static int __cdecl CopyEntryRangeBackward(int rangeStart, int sourceEnd, int destEnd, int /*unused*/)
{
    std::uintptr_t srcCursor = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(sourceEnd));
    std::uintptr_t dstCursor = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(destEnd));
    const std::uintptr_t rangeLimit = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(rangeStart));

    if (rangeLimit != srcCursor)
    {
        std::uintptr_t destWork = dstCursor + 16;   // v5
        std::uintptr_t srcWork = srcCursor + 16;    // v6

        do
        {
            *reinterpret_cast<std::uint32_t*>(dstCursor - 60) =
                *reinterpret_cast<std::uint32_t*>(srcCursor - 60);
            dstCursor -= 60;

            srcWork -= 60;
            srcCursor -= 60;
            destWork -= 60;

            std::uint32_t* head = reinterpret_cast<std::uint32_t*>(destWork - 12);
            head[0] = *reinterpret_cast<std::uint32_t*>(srcWork - 12);
            head[1] = *reinterpret_cast<std::uint32_t*>(srcWork - 8);
            head[2] = *reinterpret_cast<std::uint32_t*>(srcWork - 4);

            std::uint32_t* body = reinterpret_cast<std::uint32_t*>(destWork);
            body[0] = *reinterpret_cast<std::uint32_t*>(srcWork);
            body[1] = *reinterpret_cast<std::uint32_t*>(srcWork + 4);
            body[2] = *reinterpret_cast<std::uint32_t*>(srcWork + 8);
            body[3] = *reinterpret_cast<std::uint32_t*>(srcWork + 12);

            AssignStringSubstring(reinterpret_cast<void*>(destWork + 16), nullptr,
                                  reinterpret_cast<const void*>(srcWork + 16), 0, 0xFFFFFFFFu);
        }
        while (srcCursor != rangeLimit);
    }

    return static_cast<int>(static_cast<std::uint32_t>(dstCursor));
}

HOOK(0x007753B0, CopyEntryRangeBackward, "auto");
