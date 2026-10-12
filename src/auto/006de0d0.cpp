#include <cstdint>
#include <cstddef>
#include <cstring>
#include "hook.h"

// Copies consecutive 28-byte records from [src, srcEnd) to dst, returning the
// destination pointer just past the last record written.
static char* __cdecl GfxTextureLoader_CopyRecordRange(char* srcBegin, char* srcEnd, char* dstBegin) {
    const size_t kRecordSize = 0x1C;  // 28 bytes per texture record
    char* src = srcBegin;
    char* dst = dstBegin;
    while (src != srcEnd) {
        const char* recordSrc = src;
        char* recordDst = dst;
        src += kRecordSize;
        dst += kRecordSize;
        std::memcpy(recordDst, recordSrc, kRecordSize);
    }
    return dst;
}

HOOK(0x006DE0D0, GfxTextureLoader_CopyRecordRange, "auto");
