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

// std::_String_base::assign(src, pos, count) style helper, __thiscall, ret 12.
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies 32-byte entries (a DWORD at +0, a std::string at +4) from the range
// [begin, end) backwards into the block ending at dest. Returns the new block start.
// The original's fourth stack argument is never read, so it is not declared here.
static uint32_t* __cdecl CopyDwordStringEntriesBackward(int begin, int end, uint32_t* dest)
{
    if (begin == end)
        return dest;

    int source_entry = end;
    uint32_t* out_entry = dest;
    do
    {
        uint32_t entry_value = *reinterpret_cast<uint32_t*>(source_entry - 32);
        source_entry -= 32;
        out_entry -= 8;
        *out_entry = entry_value;
        AssignStringSubstring(
            reinterpret_cast<uint8_t*>(out_entry) + 4,
            nullptr,
            reinterpret_cast<const uint8_t*>(source_entry) + 4,
            0,
            static_cast<std::uint32_t>(-1));
    } while (source_entry != begin);

    return out_entry;
}

HOOK(0x0069B8F0, CopyDwordStringEntriesBackward, "auto");
