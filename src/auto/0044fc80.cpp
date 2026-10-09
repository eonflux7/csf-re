#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Removes the first occurrence of `value` from an int array object.
// list[0] = data pointer, list[2] = element count. Shifts the tail down by one and decrements the count.
static void __stdcall RemoveValueFromIntList(int value, uint32_t* list) {
    const uint32_t initialCount = list[2];
    if (initialCount == 0) {
        return;
    }

    int32_t* items = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(list[0]));
    uint32_t index = 0;

    while (items[index] != value) {
        ++index;
        if (index >= list[2]) {
            return;
        }
    }

    list[2] = initialCount - 1;
    if (index != initialCount - 1) {
        _memcpy(&items[index], &items[index + 1], 4 * (initialCount - 1 - index));
    }
}

HOOK(0x0044FC80, RemoveValueFromIntList, "auto");
