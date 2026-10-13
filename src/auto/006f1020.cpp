#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original 0x006F1020 (__thiscall, ret 4): fetches the child object from +0x20,
// obtains a sub-object from its slot +0x24, invokes that object's slot +0x8C
// with the argument, and returns the argument.
static int __fastcall C_00862390_ForwardArgToChildAndReturn(C_00862390* self, void* /*edx*/, int a2) {
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));

    // child->vSlot24() : returns a pointer to the sub-object (slot 0x24 / 4 = 9 in the child's table,
    // called through the child's own vtable as in the original).
    using GetSubObjectFn = void*(__fastcall*)(void* self, void* edx);
    GetSubObjectFn getSubObject = reinterpret_cast<GetSubObjectFn>(CSF_AT(void**, child, 0)[0x24 / 4]);
    void* subObject = getSubObject(child, nullptr);

    // subObject->vSlot8C(a2) : slot 0x8C / 4 = 35
    using ForwardArgFn = void(__fastcall*)(void* self, void* edx, int a2);
    ForwardArgFn forwardArg = reinterpret_cast<ForwardArgFn>(CSF_AT(void**, subObject, 0)[0x8C / 4]);
    forwardArg(subObject, nullptr, a2);

    return a2;
}

HOOK(0x006F1020, C_00862390_ForwardArgToChildAndReturn, "auto");
