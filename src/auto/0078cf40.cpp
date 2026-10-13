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

// Returns a pointer to an 8-byte sample record (two dwords) for the value
// written into the caller's scratch buffer.
CSF_FUNC(uint64_t*, __cdecl, PerfCounters_GetSampleRecord, 0x006474D0, (uint64_t* a1));

// Submits one (flag, low dword, high dword) sample to the perf counter sink.
CSF_FUNC(int, __fastcall, PerfCounters_SubmitValue, 0x006882B0, (void* self, void* edx, int a2, int a3, int a4));

// Records the sample pair held at self+4 (when flag is set, reported as the
// primary slot) and at self+8 (reported as the secondary slot).
// The object pointer is passed in ecx; the single stack argument is the flag.
static int __fastcall DataSetPerfCounters_RecordSamplePair(void* self, void* /*edx*/, int enableFlag)
{
    if (enableFlag != 0)
    {
        alignas(8) uint8_t primaryScratch[8];
        uint32_t* primaryRecord = reinterpret_cast<uint32_t*>(
            PerfCounters_GetSampleRecord(reinterpret_cast<uint64_t*>(primaryScratch)));
        PerfCounters_SubmitValue(CSF_AT(void*, self, 0x4), nullptr, 1,
                                 static_cast<int>(primaryRecord[0]),
                                 static_cast<int>(primaryRecord[1]));
    }

    alignas(8) uint8_t secondaryScratch[8];
    uint32_t* secondaryRecord = reinterpret_cast<uint32_t*>(
        PerfCounters_GetSampleRecord(reinterpret_cast<uint64_t*>(secondaryScratch)));
    return PerfCounters_SubmitValue(CSF_AT(void*, self, 0x8), nullptr, enableFlag,
                                    static_cast<int>(secondaryRecord[0]),
                                    static_cast<int>(secondaryRecord[1]));
}

HOOK(0x0078CF40, DataSetPerfCounters_RecordSamplePair, "auto");
