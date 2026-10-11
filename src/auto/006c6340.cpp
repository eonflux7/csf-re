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

// Grows the backing storage of an S_0041e5f0 growable array (fields: +0 data, +4 capacity, +8 count, +0xC grow step).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Referenced by the original's call site at 0x6C6355.
static const void* const kGrowArrayVtblRef = CSF_VTBL(0x0041E5F0);

// Appends `value` to the growable array `array` and returns `value`.
static int __stdcall S_0041e5f0_PushBackValue(int value, S_0041e5f0* array)
{
    if (array->field_0008 >= array->field_0004)
        S_0041e5f0_GrowArrayCapacity(array, nullptr, static_cast<int>(array->field_000c));

    uint32_t index = array->field_0008;
    int* slot = reinterpret_cast<int*>(array->field_0000 + 4 * index);
    array->field_0008 = index + 1;
    *slot = value;
    return value;
}

HOOK(0x006C6340, S_0041e5f0_PushBackValue, "auto");
