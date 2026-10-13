#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x18 of the child object at this->+0x24 (slot 6), __thiscall with one stack arg.
typedef int(__fastcall* ChildSlot6Fn)(void* self, void* edx, uint32_t* a2);

static int __fastcall C_008632c0_CopyTripletAndForwardToChild(C_008632c0* self, void* /*edx*/, uint32_t* a2)
{
    // Copy the three dwords of the argument block into this+0x0C..0x14.
    CSF_AT(uint32_t, self, 0x0C) = a2[0];
    CSF_AT(uint32_t, self, 0x10) = a2[1];
    CSF_AT(uint32_t, self, 0x14) = a2[2];

    // Forward the original argument to the child object's virtual slot 6.
    void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x24));
    ChildSlot6Fn forwardFn = reinterpret_cast<ChildSlot6Fn>(CSF_AT(void**, child, 0)[6]);
    int result = forwardFn(child, nullptr, a2);

    CSF_AT(uint8_t, self, 0x38) = 0;
    return result;
}

HOOK(0x706700, C_008632c0_CopyTripletAndForwardToChild, "auto");
