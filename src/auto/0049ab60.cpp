#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Growth routine for the event-record array, called when the record count reaches capacity.
CSF_FUNC(unsigned int, __fastcall, MissionScript_GrowEventRecords, 0x00413570, (void* self, void* edx, int a2));

// Vtable of the record-list sub-object (S_00413570), referenced by the original's call site.
[[maybe_unused]] static const void* const kEventRecordListVtbl = CSF_VTBL(0x00413570);

// Appends a 12-byte event record (two dwords copied from a2 + 0x20 / a2 + 0x24) to the
// sub-object at this + 0x3F0 when flag 0x10 is set in field_0170. Returns the second copied value,
// or 0 when the flag is not set (the original returns an unset eax in that case).
static int __fastcall C_00833908_AppendEventRecord(C_00833908* self, void* /*edx*/, int a2)
{
    int result = 0;
    if ((self->field_0170 & 0x10) != 0)
    {
        void* recordList = reinterpret_cast<uint8_t*>(self) + 0x3F0;
        uint32_t capacity = CSF_AT(uint32_t, self, 0x3F4);
        if (CSF_AT(uint32_t, self, 0x3F8) >= capacity)
            MissionScript_GrowEventRecords(recordList, nullptr, CSF_AT(int, self, 0x3FC));

        uint32_t index = CSF_AT(uint32_t, self, 0x3F8);
        uint32_t recordsAddr = CSF_AT(uint32_t, self, 0x3F0);
        CSF_AT(uint32_t, self, 0x3F8) = index + 1;

        uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(recordsAddr)) + 12u * index;
        uint8_t* source = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
        CSF_AT(uint32_t, record, 4) = CSF_AT(uint32_t, source, 0x20);
        result = static_cast<int>(CSF_AT(uint32_t, source, 0x24));
        CSF_AT(uint32_t, record, 8) = static_cast<uint32_t>(result);
    }
    return result;
}

HOOK(0x49AB60, C_00833908_AppendEventRecord, "auto");
