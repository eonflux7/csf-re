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

CSF_FUNC(int, __fastcall, Table_FindIndex, 0x0075CE20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00692b80_FindIndexOfValue, 0x00462A80, (S_00692b80* self, void* , int value));

// Writes the three 32-bit values of the record stored under (a2, a4) into arg0.
// Returns arg0. Missing keys yield zeros.
static int* __fastcall C_0086c288_GetPointPosition(C_0086c288* self, void* /*edx*/, int* arg0, int a2, int a4)
{
    int valueX = 0;
    int valueY = 0;
    int valueZ = 0;

    int tableIndex = Table_FindIndex(self, nullptr, a2);
    if (tableIndex != -1)
    {
        // Each table entry is 72 bytes; the value map lives at entry+0x38.
        uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0008) + 72 * tableIndex;
        int valueIndex = S_00692b80_FindIndexOfValue(reinterpret_cast<S_00692b80*>(entry), nullptr, a4);
        if (valueIndex != -1)
        {
            uint32_t recordsBase = CSF_AT(uint32_t, entry, 0x38);
            uint32_t recordAddr = recordsBase + 44u * static_cast<uint32_t>(valueIndex);
            if (recordAddr != 0)
            {
                uint8_t* record = reinterpret_cast<uint8_t*>(recordAddr);
                valueX = CSF_AT(int, record, 0x4);
                valueY = CSF_AT(int, record, 0x8);
                valueZ = CSF_AT(int, record, 0xC);
            }
        }
    }

    arg0[0] = valueX;
    arg0[1] = valueY;
    arg0[2] = valueZ;
    return arg0;
}

HOOK(0x75D020, C_0086c288_GetPointPosition, "auto");
