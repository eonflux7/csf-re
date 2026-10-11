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

// CRT qsort, cdecl, 4 stack args. Bound by address.
CSF_FUNC(void, __cdecl, CRT_Qsort, 0x7AEAE0,
         (void* base, size_t count, size_t elementSize, int (__cdecl* compare)(const void*, const void*)));

// Record comparator at 0x513FB0 (passed as a value, so bound as an address).
typedef int (__cdecl* RecordCompareFn)(const void*, const void*);

static void __cdecl SaveGames_SortRecordArray(int rangeHandle)
{
    // rangeHandle points to a { ?, begin (+4), end (+8) } record range.
    uint8_t* range = reinterpret_cast<uint8_t*>(rangeHandle);

    void* recordBegin = CSF_AT(void*, range, 0x04);
    if (recordBegin)
    {
        int byteSpan = CSF_AT(int, range, 0x08) - static_cast<int>(reinterpret_cast<uintptr_t>(recordBegin));
        int recordCount = byteSpan / 200;
        if (recordCount)
        {
            CRT_Qsort(recordBegin,
                      static_cast<size_t>(recordCount),
                      0xC8u,
                      reinterpret_cast<RecordCompareFn>(CSF_ADDR_FN(0x513FB0)));
        }
    }
}

HOOK(0x514140, SaveGames_SortRecordArray, "auto");
