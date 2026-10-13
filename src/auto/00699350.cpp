#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// BucketEntry_Match: __thiscall-style callee taking (a2, a3) on the stack; ret 8.
CSF_FUNC(int, __fastcall, BucketEntry_Match, 0x00699250, (void* self, void* edx, unsigned int a2, uint8_t* a3));

// Vtable referenced by the original's call site (S_00699250's vtable).
[[maybe_unused]] static const auto kProbeEntryVtable = CSF_VTBL(0x00699250);

// Probes a bucket entry with the given key and returns the match result,
// but only if the key's high byte is nonzero; otherwise returns 0.
static int __fastcall TableProbe_EntryHandler(S_00699250* self, void* /*edx*/, unsigned int a2)
{
    int result = BucketEntry_Match(self, nullptr, a2, reinterpret_cast<uint8_t*>(&a2) + 3);

    // The original re-reads the high byte after the call, since the callee may write it.
    uint8_t highByte = *(reinterpret_cast<uint8_t*>(&a2) + 3);
    if (highByte == 0)
        return 0;
    return result;
}

HOOK(0x699350, TableProbe_EntryHandler, "auto");
