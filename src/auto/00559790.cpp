#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee at 0x00559520: __thiscall-style copy of the 16-byte sub-record at +4, takes one stack arg (ret 4).
CSF_FUNC(int, __fastcall, MpRules_CopySabotageRecordTail, 0x00559520, (void* self, void* edx, void* a4a));

// Copies the 20-byte sabotage records in [begin, end) backwards into the block ending at dst.
// Each record's first dword is copied directly; its tail (+4) is copied by MpRules_CopySabotageRecordTail.
// Returns the new (lowest) destination record, or dst when the source range is empty.
static uint32_t* __cdecl MpRules_CopySabotageRecordsBackward(uint8_t* begin, uint8_t* end, uint8_t* dst, int /*unused*/)
{
    if (begin == end)
        return reinterpret_cast<uint32_t*>(dst);

    uint8_t* src = end;
    uint8_t* out = dst;
    do
    {
        const uint32_t firstDword = *reinterpret_cast<uint32_t*>(src - 20);
        src -= 20;
        out -= 20;
        *reinterpret_cast<uint32_t*>(out) = firstDword;
        MpRules_CopySabotageRecordTail(out + 4, nullptr, src + 4);
    } while (src != begin);

    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x00559790, MpRules_CopySabotageRecordsBackward, "auto");
