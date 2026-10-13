#include <cstdint>
#include "hook.h"

// Calls `constructor` once per element of a contiguous array, advancing `base` by
// `elementSize` bytes each step. `constructor` is a __thiscall with no stack
// arguments (ecx = element address). Returns the last constructor result, or
// elementCount - 1 when the loop body never runs.
static int __stdcall SceneLoader_ConstructArrayElements(int base, int elementSize, int elementCount,
                                                        int(__thiscall* constructor)(int))
{
    // Original computes elementCount - 1 and tests the sign (js).
    int result = static_cast<int>(static_cast<uint32_t>(elementCount) - 1u);
    if (result >= 0)
    {
        int remaining = elementCount;
        int elementAddress = base;
        do
        {
            // Same as the original: ecx = element address, no stack arguments.
            auto callConstructor = reinterpret_cast<int(__fastcall*)(void*, void*)>(constructor);
            result = callConstructor(reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(elementAddress))),
                                     nullptr);
            elementAddress = static_cast<int>(static_cast<uint32_t>(elementAddress) +
                                              static_cast<uint32_t>(elementSize));
            --remaining;
        } while (remaining != 0);
    }
    return result;
}

HOOK(0x4011C0, SceneLoader_ConstructArrayElements, "auto");
