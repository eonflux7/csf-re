#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Nested-record copier at 0x40ACC0: __thiscall (this in ecx), one stack argument (source).
CSF_FUNC(int, __fastcall, CopyNestedRecord, 0x0040ACC0, (void* self, void* edx, int a2));

// Copies records of 7 dwords (28 bytes) from [srcBegin, srcEnd) to dstBegin.
// The first three dwords are copied directly; the sub-object at +12 is copied via CopyNestedRecord.
// Returns the one-past-the-end destination pointer, or dstBegin when the source range is empty.
static uint32_t* __cdecl CopyRecordRange(uint32_t* srcBegin, uint32_t* srcEnd, uint32_t* dstBegin)
{
    if (srcBegin == srcEnd)
        return dstBegin;

    uint32_t* src = srcBegin;
    uint32_t* dst = dstBegin;
    do
    {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        CopyNestedRecord(reinterpret_cast<void*>(dst + 3), nullptr,
                         static_cast<int>(reinterpret_cast<uintptr_t>(src + 3)));
        src += 7;
        dst += 7;
    } while (src != srcEnd);

    return dst;
}

HOOK(0x0040AE10, CopyRecordRange, "auto");
