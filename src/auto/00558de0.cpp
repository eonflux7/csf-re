#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee at 0x00558BA0: __thiscall-style copy of the 16-byte sub-record at +4, takes one stack arg (ret 4).
CSF_FUNC(int, __fastcall, MpRules_CopyRecordTail, 0x00558BA0, (void* self, void* edx, int a5));

// Copies the 20-byte records in [begin, end) backwards into the block ending at dst.
// Each record's first dword is copied directly; its tail (+4) is copied by MpRules_CopyRecordTail.
// Returns the new (lowest) destination record, or dst when the source range is empty.
static uint32_t* __cdecl MpRules_CopyRecordsBackward(uint8_t* begin, uint8_t* end, uint8_t* dst)
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
        MpRules_CopyRecordTail(out + 4, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(src + 4)));
    } while (src != begin);

    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x00558DE0, MpRules_CopyRecordsBackward, "auto");
