#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. Processes one data-set perf counter
// entry and returns a char* result.
CSF_FUNC(char*, __fastcall, DataSetPerfCounters_ProcessEntry, 0x0078BD50, (void* self, void* edx, int a2));

// Forwards a2 to DataSetPerfCounters_ProcessEntry (with a1 as the this pointer) when
// a1 is non-null; otherwise nothing is done. The original returned an uninitialized
// eax in that case; this returns nullptr instead.
static char* __stdcall NetPerf_ForwardToEntryIfNonNull(char* a1, uint32_t* a2)
{
    if (a1)
        return DataSetPerfCounters_ProcessEntry(a1, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(a2)));
    return nullptr;
}

HOOK(0x0078BEB0, NetPerf_ForwardToEntryIfNonNull, "auto");
