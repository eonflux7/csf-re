#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// qsort from the CRT, bound by address.
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0,
         (void* base, std::size_t count, std::size_t size,
          int (__cdecl* compare)(const void*, const void*)));

// Element comparator used by the table sort.
static int __cdecl CoreSortedTable_CompareEntries_Unused_Placeholder(const void*, const void*);

static void CoreSortedTable_SortEntries(int a1)
{
    uint8_t* table = reinterpret_cast<uint8_t*>(a1);
    void* entriesBegin = CSF_AT(void*, table, 0x04);
    if (entriesBegin)
    {
        // Each entry is 8 bytes: element count = (end - begin) >> 3.
        int32_t byteSpan = (int32_t)CSF_AT(uint32_t, table, 0x08) - (int32_t)reinterpret_cast<uintptr_t>(entriesBegin);
        int32_t entryCount = byteSpan >> 3;
        if (entryCount != 0)
        {
            _qsort(entriesBegin,
                   (std::size_t)entryCount,
                   8u,
                   reinterpret_cast<int (__cdecl*)(const void*, const void*)>(CSF_ADDR_FN(0x00698E20)));
        }
    }
}

HOOK(0x699220, CoreSortedTable_SortEntries, "auto");
