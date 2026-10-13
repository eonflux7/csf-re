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

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

[[maybe_unused]] static void* const kGrowArrayVtable = CSF_VTBL(0x0041E5F0);

// Copies the 32-bit value of every entry whose enabled flag (byte +4) is set
// into the output array `a2`, growing it as needed. Entries are 8 bytes each,
// starting at this+0x2C, and there are field_0028 of them.
static int __fastcall CInputDirectInput_CopyEnabledStatesToArray(CInputDirectInput* self, void* /*edx*/, S_0041e5f0* a2)
{
    int index = 0;
    int result = 0;
    a2->field_0008 = 0;
    if ((int)self->field_0028 > 0)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 0x2C;
        do
        {
            if (CSF_AT(uint8_t, entry, 4) != 0)
            {
                if (a2->field_0008 >= a2->field_0004)
                {
                    S_0041e5f0_GrowArrayCapacity(a2, nullptr, (int)a2->field_000c);
                }
                uint32_t position = a2->field_0008;
                uint32_t* items = reinterpret_cast<uint32_t*>(a2->field_0000);
                a2->field_0008 = position + 1;
                items[position] = CSF_AT(uint32_t, entry, 0);
            }
            result = (int)self->field_0028;
            ++index;
            entry += 8;
        } while (index < result);
    }
    return result;
}

HOOK(0x713970, CInputDirectInput_CopyEnabledStatesToArray, "auto");
