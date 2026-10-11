#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));

// Appends a type-3 entry to the button array (this+0x00) and copies a 2-dword payload
// (a2+4, a2+8) into a 12-byte event record in the sub-object at this+0x20.
// Returns the payload dword at a2+8.
static int __fastcall S_0041e5f0_PushTypeThreeEvent(S_0041e5f0* self, void* /*edx*/, int a2)
{
    if (self->field_0008 >= self->field_0004)
        GrowButtonArray(self, nullptr, self->field_000c);

    uint32_t buttonIndex = self->field_0008;
    uint32_t* buttonSlot = reinterpret_cast<uint32_t*>(self->field_0000 + 4 * buttonIndex);
    self->field_0008 = buttonIndex + 1;
    *buttonSlot = 3;

    S_00413570* events = reinterpret_cast<S_00413570*>(reinterpret_cast<uint8_t*>(self) + 0x20);
    if (events->field_0008 >= events->field_0004)
        MissionScript_GrowEventRecords(events, nullptr, events->field_000c);

    uint32_t eventIndex = events->field_0008;
    uint32_t recordsBase = events->field_0000;
    events->field_0008 = eventIndex + 1;
    uint8_t* record = reinterpret_cast<uint8_t*>(recordsBase + 12 * eventIndex);

    uint8_t* payload = reinterpret_cast<uint8_t*>(a2);
    CSF_AT(uint32_t, record, 4) = CSF_AT(uint32_t, payload, 4);
    int result = CSF_AT(int, payload, 8);
    CSF_AT(uint32_t, record, 8) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x004C5150, S_0041e5f0_PushTypeThreeEvent, "auto");
