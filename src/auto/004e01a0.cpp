#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (bound by address, signatures as given in the packet).
CSF_FUNC(int, __cdecl, PendingRange_Flush, 0x004DF6C0, (int a1, int a2, int a3, int a4, float a5, int a6));
CSF_FUNC(unsigned int, __fastcall, PendingList_PushPair, 0x004E0130, (void* self, void* edx, uint32_t* a3));

// S_004e0130 vtable reference (the original's call at 0x4E01BD names this class's vtable).
[[maybe_unused]] static const auto s_pendingListVtbl = CSF_VTBL(0x004E0130);

// Pushes the pair (*a6, a3) into the list, then flushes the entries that were
// pending before the push (every 8-byte entry except the newest one).
static int __fastcall PushPairAndFlushPending(S_004e0130* self, void* /*edx*/, std::uint32_t* a6, int a3)
{
    uint32_t pair[2];
    pair[0] = *a6;
    pair[1] = static_cast<uint32_t>(a3);
    PendingList_PushPair(self, nullptr, pair);

    // The original clears the low byte of the a6 argument slot before reusing it.
    int clearedA6 = static_cast<int>(reinterpret_cast<uintptr_t>(a6) & ~static_cast<uintptr_t>(0xFF));

    int result = static_cast<int>(self->field_0008);
    int begin = static_cast<int>(self->field_0004);
    if (begin != result)
    {
        int lastEntry = result - 8;
        result = (lastEntry - begin) >> 3;
        if (result > 0)
        {
            int lastKey = CSF_AT(int, reinterpret_cast<void*>(lastEntry), 0);
            float lastValue = CSF_AT(float, reinterpret_cast<void*>(lastEntry), 4);
            return PendingRange_Flush(begin, result, 0, lastKey, lastValue, clearedA6);
        }
    }
    return result;
}

HOOK(0x004E01A0, PushPairAndFlushPending, "auto");
