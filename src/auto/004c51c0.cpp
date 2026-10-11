#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));

// Appends a value (a2) to this->field_0000 array, then appends a 12-byte
// record copied from a3 (dwords +4 and +8) to the event array at this+0x20.
static int __fastcall S_0041e5f0_AppendPointAndEventRecord(S_0041e5f0* self, void* /*edx*/, int a2, int a3)
{
    if (self->field_0008 >= self->field_0004)
        GrowButtonArray(self, nullptr, static_cast<int>(self->field_000c));

    uint32_t slotIndex = self->field_0008;
    int* slot = reinterpret_cast<int*>(self->field_0000 + 4 * slotIndex);
    self->field_0008 = slotIndex + 1;
    *slot = a2;

    S_00413570* eventList = reinterpret_cast<S_00413570*>(reinterpret_cast<uint8_t*>(self) + 0x20);
    if (eventList->field_0008 >= eventList->field_0004)
        MissionScript_GrowEventRecords(eventList, nullptr, static_cast<int>(eventList->field_000c));

    uint32_t recordIndex = eventList->field_0008;
    uint32_t recordBase = eventList->field_0000;
    eventList->field_0008 = recordIndex + 1;
    void* record = reinterpret_cast<void*>(recordBase + 12 * recordIndex);

    void* source = reinterpret_cast<void*>(a3);
    CSF_AT(uint32_t, record, 4) = CSF_AT(uint32_t, source, 4);
    uint32_t result = CSF_AT(uint32_t, source, 8);
    CSF_AT(uint32_t, record, 8) = result;
    return static_cast<int>(result);
}

HOOK(0x004C51C0, S_0041e5f0_AppendPointAndEventRecord, "auto");
