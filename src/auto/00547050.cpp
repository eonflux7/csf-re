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
CSF_FUNC(void*, __fastcall, MenuExtras_StringAssignSubstr, 0x488830,
         (void* self, void* edx, const void* src, int pos, int count));

// Copies 32-byte entries (a DWORD at +0, a std::string at +4) from the range
// [begin, end) backwards into the block ending at dest. Returns the new block start.
static uint32_t* __cdecl MenuExtras_CopyEntriesBackward(int begin, int end, uint32_t* dest)
{
    if (begin == end)
        return dest;

    int src = end;
    uint32_t* out = dest;
    do
    {
        uint32_t value = *reinterpret_cast<uint32_t*>(src - 32);
        src -= 32;
        out -= 8;
        *out = value;
        MenuExtras_StringAssignSubstr(
            reinterpret_cast<uint8_t*>(out) + 4,
            nullptr,
            reinterpret_cast<const uint8_t*>(src) + 4,
            0,
            -1);
    } while (src != begin);

    return out;
}

HOOK(0x00547050, MenuExtras_CopyEntriesBackward, "auto");
