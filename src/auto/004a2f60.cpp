#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the argument in this->field_003c, then forwards it through a stack
// frame to the virtual method at slot 0x4C of the child sub-object at
// (this->field_0008 + 4). Returns the child's result.
static int __fastcall C_008344f0_StoreValueAndForwardToChild(C_008344f0* self, void* /*edx*/, int a2)
{
    self->field_003c = static_cast<uint32_t>(a2);

    // Sub-object embedded at +4 inside the object pointed to by field_0008.
    uint8_t* childSubobject = reinterpret_cast<uint8_t*>(self->field_0008) + 4;

    // Stack block the original keeps at [ebp-8]: an int value at +0 and a flag byte at +4.
    alignas(8) uint8_t argFrame[8];
    CSF_AT(int, argFrame, 0) = a2;
    CSF_AT(uint8_t, argFrame, 4) = 1;

    // Virtual call through vtable slot 0x4C / 4 = 19 on the child object.
    using ChildMethod = int(__fastcall*)(void*, void*, int*);
    ChildMethod childMethod = reinterpret_cast<ChildMethod>(CSF_AT(void**, childSubobject, 0)[0x4C / 4]);
    return childMethod(childSubobject, nullptr, reinterpret_cast<int*>(argFrame));
}

HOOK(0x4A2F60, C_008344f0_StoreValueAndForwardToChild, "auto");
