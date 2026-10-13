#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x006DCC30 (__thiscall, ret 4): takes the child object stored at +0x94,
// calls its slot +0x14 to obtain a sub-object, invokes that sub-object's slot +0x8C
// with the argument, and returns the argument.
static int __fastcall C_00861508_ForwardArgToChildAndReturn(C_00861508* self, void* /*edx*/, int a2) {
    void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0094));

    // child->vSlot14() : slot 0x14 / 4 = 5 in the child's table, returns a sub-object pointer.
    using GetSubObjectFn = void*(__fastcall*)(void* self, void* edx);
    GetSubObjectFn getSubObject = reinterpret_cast<GetSubObjectFn>(CSF_AT(void**, child, 0)[0x14 / 4]);
    void* subObject = getSubObject(child, nullptr);

    // subObject->vSlot8C(a2) : slot 0x8C / 4 = 35
    using ForwardArgFn = void(__fastcall*)(void* self, void* edx, int a2);
    ForwardArgFn forwardArg = reinterpret_cast<ForwardArgFn>(CSF_AT(void**, subObject, 0)[0x8C / 4]);
    forwardArg(subObject, nullptr, a2);

    return a2;
}

HOOK(0x006DCC30, C_00861508_ForwardArgToChildAndReturn, "auto");
