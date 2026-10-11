#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void*, int additionalCapacity));
CSF_FUNC(void, __fastcall, C_Unknown_ClearField0048, 0x004E5270, (void* self, void*));
CSF_FUNC(char, __fastcall, EventRecords_FindExisting, 0x004E73F0, (void* self, void* edx, int a2));

// Adds a 12-byte event record (copies two dwords from the source) into the
// first free slot (key == -1) or appends a new one, growing if needed.
static void __fastcall C_0083eb70_AddEventRecord(C_0083eb70* self, void* /*edx*/, int a2)
{
    if (EventRecords_FindExisting(self, nullptr, a2))
        return;

    uint32_t count = self->field_0098;
    uint32_t index = 0;

    if (count != 0) {
        uint32_t base = self->field_0090;
        uint32_t* key = reinterpret_cast<uint32_t*>(base + 4);
        while (*key != 0xFFFFFFFFu) {
            ++index;
            key += 3;
            if (index >= count)
                goto append;
        }
        uint8_t* rec = reinterpret_cast<uint8_t*>(base + 12 * index);
        CSF_AT(uint32_t, rec, 4) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 4);
        CSF_AT(uint32_t, rec, 8) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 8);
        C_Unknown_ClearField0048(self, nullptr);
        return;
    }

append:
    if (self->field_0098 >= self->field_0094)
        S_00413570_GrowEventRecords(reinterpret_cast<S_00413570*>(&self->field_0090), nullptr, (int)self->field_009c);

    uint32_t n = self->field_0098;
    uint8_t* rec = reinterpret_cast<uint8_t*>(self->field_0090 + 12 * n);
    self->field_0098 = n + 1;
    CSF_AT(uint32_t, rec, 4) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 4);
    CSF_AT(uint32_t, rec, 8) = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 8);
    C_Unknown_ClearField0048(self, nullptr);
}

HOOK(0x004E87D0, C_0083eb70_AddEventRecord, "auto");
