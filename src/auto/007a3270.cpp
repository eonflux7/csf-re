#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DataSetPerfCounters_RecordSamplePair, 0x0078CF40, (void*, void*, int));
static int __stdcall PerfSample_007a3270(int, int, int, int, int enable) {
    return DataSetPerfCounters_RecordSamplePair(reinterpret_cast<void*>(CSF_ADDR_DATA(0x008BABE0)), nullptr, enable);
}
HOOK(0x007A3270, PerfSample_007a3270, "auto");
