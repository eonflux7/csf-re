#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee at 0x699250: matches a bucket entry for (a2, a3); returns nonzero on success and sets *a3.
CSF_FUNC(int, __fastcall, BucketEntry_Match, 0x00699250, (void* self, void* edx, unsigned int a2, uint8_t* a3));

// Vtable of the bucket entry class (referenced by the call site at 0x69B7F0).
static auto const g_BucketEntryVtbl = CSF_VTBL(0x00699250);

// Number of significant bits in (arg4 >> 1); -1 when arg4 is zero (wraps the bucket index as in the original).
static int ComputeBucketIndex(unsigned int arg4)
{
    if (arg4 == 0)
        return -1;

    unsigned int shifted = arg4 >> 1;
    int bitCount = 0;
    while (shifted != 0) {
        shifted >>= 1;
        ++bitCount;
    }
    return bitCount;
}

static int __fastcall ResolveBucketEntryOrDefault(void* self, void* /*edx*/, unsigned int a2, unsigned int arg4, uint8_t* a3)
{
    int bucketIndex = ComputeBucketIndex(arg4);

    uint8_t* bucketArray = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
    void* entry = *reinterpret_cast<void**>(bucketArray + static_cast<ptrdiff_t>(32) * bucketIndex);

    if (entry != nullptr) {
        int result = BucketEntry_Match(entry, nullptr, a2, a3);
        if (result != 0 && *a3 != 0)
            return result;
    }

    if (CSF_AT(uint32_t, self, 0x2C) < 8u)
        return static_cast<int>(reinterpret_cast<uintptr_t>(&CSF_AT(uint32_t, self, 0x18)));

    return CSF_AT(int, self, 0x18);
}

HOOK(0x0069B7B0, ResolveBucketEntryOrDefault, "auto");
