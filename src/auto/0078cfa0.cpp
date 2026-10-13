#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, PerfCounters_RegisterCounterEntry, 0x0078CEA0,
         (void* self, void* edx, void** counterSlot, char* counterName));

static void __fastcall PerfCounters_InitMessageCounters(S_0078cea0* self, void* /*edx*/)
{
    self->field_0010 = 1;

    PerfCounters_RegisterCounterEntry(
        self, nullptr,
        reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 0x4),
        const_cast<char*>(CSF_STR(0x00872CA8, "UpdatesThatTrigeredMessages")));

    PerfCounters_RegisterCounterEntry(
        self, nullptr,
        reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + 0x8),
        const_cast<char*>(CSF_STR(0x00872CC4, "MessagesSent")));
}

HOOK(0x0078CFA0, PerfCounters_InitMessageCounters, "auto");
