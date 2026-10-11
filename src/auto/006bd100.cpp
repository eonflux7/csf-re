#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// Appends `value` to the dynamic int array at `array` (data at +0, capacity at +4,
// count at +8, growth step at +0xC) and returns the value.
static int __cdecl PushValueToDynamicArray(int value, S_0041e5f0* array)
{
    if (array->field_0008 >= array->field_0004)
    {
        S_0041e5f0_GrowArrayCapacity(array, nullptr, array->field_000c);
    }

    // Re-read the count and data pointer after a possible grow.
    uint32_t index = array->field_0008;
    int* slots = reinterpret_cast<int*>(array->field_0000);
    array->field_0008 = index + 1;
    slots[index] = value;
    return value;
}

HOOK(0x006BD100, PushValueToDynamicArray, "auto");
