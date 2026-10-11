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

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* edx, int additionalCapacity));
CSF_FUNC(int, __fastcall, FindEventRecordIndex, 0x00444F80, (void* self, void* edx, int a2));

// Appends the event record pointed to by a2 (fields +4 and +8) to this object's
// record list when the enable flag is set and the record's +8 value is non-negative,
// unless FindEventRecordIndex already locates it (returns something other than -1).
static char __fastcall C_0083e248_AppendEventRecord(C_0083e248* self, void* /*edx*/, int a2)
{
    int result = 0;
    if (self->field_002d != 0)
    {
        uint8_t* record = reinterpret_cast<uint8_t*>(a2);
        result = CSF_AT(int, record, 8);
        if (result >= 0)
        {
            S_00413570* recordList = reinterpret_cast<S_00413570*>(&self->field_0034);
            result = FindEventRecordIndex(recordList, nullptr, a2);
            if (result == -1)
            {
                if (recordList->field_0008 >= recordList->field_0004)
                {
                    (void)CSF_VTBL(0x00413570);
                    S_00413570_GrowEventRecords(recordList, nullptr, recordList->field_000c);
                }
                uint32_t index = recordList->field_0008;
                uint32_t base = recordList->field_0000;
                recordList->field_0008 = index + 1;
                uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(base + 12 * index));
                CSF_AT(uint32_t, entry, 4) = CSF_AT(uint32_t, record, 4);
                result = CSF_AT(int, record, 8);
                CSF_AT(int, entry, 8) = result;
            }
        }
    }
    return static_cast<char>(result);
}

HOOK(0x004E3DA0, C_0083e248_AppendEventRecord, "auto");
