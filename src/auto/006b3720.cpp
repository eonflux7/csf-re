#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Loops over the child pointer array (field_0018, count field_001c). Each non-null child gets
// its slot +0x30 method called with a2. Then sets or clears bit 0x80 in field_0064
// depending on the low byte of a2 and returns the new value.
static uint32_t __fastcall C_00860288_PropagateFlagToChildren(C_00860288* self, void* /*edx*/, int a2)
{
    for (unsigned int i = 0; i < self->field_001c; ++i)
    {
        // The array and count are re-read every iteration, as the original does.
        uint32_t* childSlots = reinterpret_cast<uint32_t*>(self->field_0018);
        void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childSlots[i]));
        if (child)
        {
            // Child type is unknown: slot +0x30 is vtbl[12], called __thiscall with a2 on the stack.
            using ChildSlot30Fn = void(__fastcall*)(void* self, void* edx, int a2);
            ChildSlot30Fn fn = reinterpret_cast<ChildSlot30Fn>(CSF_AT(void**, child, 0)[0x30 / 4]);
            fn(child, nullptr, a2);
        }
    }

    uint32_t flags = self->field_0064;
    uint32_t result;
    if (static_cast<uint8_t>(a2))
        result = flags | 0x80;
    else
        result = flags & 0xFFFFFF7Fu;

    self->field_0064 = result;
    return result;
}

HOOK(0x6B3720, C_00860288_PropagateFlagToChildren, "auto");
