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

// GrowButtonArray: __thiscall (callee pops 4), receives the array as ecx and the growth amount as its one stack argument.
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));

// Searches the array for `value`. If found, returns its index. Otherwise appends it
// (growing the storage first when the count has reached capacity) and returns the new count.
static unsigned int __stdcall ButtonArray_FindOrAppend(int value, S_0041e5f0* array)
{
    // Only a reference to the class vtable; no runtime effect.
    (void)CSF_VTBL(0x0041E5F0);

    const uint32_t countAtEntry = array->field_0008;

    if (countAtEntry != 0) {
        int32_t* slots = reinterpret_cast<int32_t*>(array->field_0000);
        unsigned int index = 0;
        while (true) {
            if (slots[index] == value) {
                return index;
            }
            ++index;
            if (index >= array->field_0008) {
                break;
            }
        }
    }

    // Not found: grow storage if the entry count reached capacity, then append.
    if (countAtEntry >= array->field_0004) {
        GrowButtonArray(array, nullptr, static_cast<int>(array->field_000c));
    }

    const uint32_t currentCount = array->field_0008;
    int32_t* appendSlot = reinterpret_cast<int32_t*>(array->field_0000) + currentCount;
    const unsigned int newCount = currentCount + 1;
    array->field_0008 = newCount;
    *appendSlot = value;
    return newCount;
}

HOOK(0x450F50, ButtonArray_FindOrAppend, "auto");
