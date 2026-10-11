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

// Binary search over a sorted array of 20-byte records owned by C_0083e9d0.
// The search key is the dword at +4 of the argument record (a2); each record's
// comparison dword sits at +12 of the record. Returns the matching index,
// or -1 when the list is empty or the key is not present.
static int __fastcall C_0083e9d0_FindRecordIndexByKey(C_0083e9d0* self, void* /*edx*/, int a2)
{
    uint32_t count = self->field_0028;
    int result = -1;

    if (count != 0)
    {
        int key = CSF_AT(int, reinterpret_cast<uint8_t*>(a2), 4);
        int low = 0;
        int high = static_cast<int>(count) - 1;

        if (high >= 0)
        {
            uint8_t* records = reinterpret_cast<uint8_t*>(self->field_0020);
            do
            {
                result = (high + low) / 2;
                int recordKey = CSF_AT(int, records, result * 20 + 12);

                if (key <= recordKey)
                {
                    if (key >= recordKey)
                        return result;
                    high = result - 1;
                }
                else
                {
                    low = result + 1;
                }
            } while (low <= high);

            return -1;
        }
    }

    return result;
}

HOOK(0x4E58E0, C_0083e9d0_FindRecordIndexByKey, "auto");
