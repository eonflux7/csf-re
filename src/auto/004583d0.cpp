#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a range of 40-byte icon-state records from [srcBegin, srcEnd) to dst.
// Per record: the first 28 bytes are copied, then the two dwords that follow (the
// 4-byte values at record offsets 0x20 and 0x24) are copied through the
// "tail" pointers, which sit at record offset 0x24 and are read at [-1] and [0].
// Dword 0x1C of each destination record is left untouched.
// Returns the destination position past the last record written.
static uint32_t* __cdecl HudStatusIcons_CopyStateRecords(uint8_t* srcBegin, uint8_t* srcEnd, uint32_t* dst)
{
    uint8_t* src = srcBegin;
    uint8_t* out = reinterpret_cast<uint8_t*>(dst);

    if (srcBegin != srcEnd)
    {
        do
        {
            std::memcpy(out, src, 0x1C);

            uint32_t* srcTail = reinterpret_cast<uint32_t*>(src + 0x24);
            uint32_t* dstTail = reinterpret_cast<uint32_t*>(out + 0x24);
            dstTail[-1] = srcTail[-1];
            dstTail[0] = srcTail[0];

            src += 0x28;
            out += 0x28;
        } while (src != srcEnd);
    }

    return reinterpret_cast<uint32_t*>(out);
}

HOOK(0x4583D0, HudStatusIcons_CopyStateRecords, "auto");
