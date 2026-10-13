#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x006F3080 (__thiscall, ret 4): takes the object stored at +0x20,
// calls its slot +0x14 (no arguments) to obtain a sub-object, invokes that
// sub-object's slot +0x8C with the argument, and returns the argument.
static int __fastcall C_00862640_ForwardArgThroughFieldAndReturn(C_00862640* self, void* /*edx*/, int a2) {
    void* fieldObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020));

    // fieldObject->vSlot14() : slot 0x14 / 4 = 5, returns a sub-object pointer.
    using GetSubObjectFn = void*(__fastcall*)(void* self, void* edx);
    GetSubObjectFn getSubObject = reinterpret_cast<GetSubObjectFn>(CSF_AT(void**, fieldObject, 0)[0x14 / 4]);
    void* subObject = getSubObject(fieldObject, nullptr);

    // subObject->vSlot8C(a2) : slot 0x8C / 4 = 35
    using ForwardArgFn = void(__fastcall*)(void* self, void* edx, int a2);
    ForwardArgFn forwardArg = reinterpret_cast<ForwardArgFn>(CSF_AT(void**, subObject, 0)[0x8C / 4]);
    forwardArg(subObject, nullptr, a2);

    return a2;
}

HOOK(0x006F3080, C_00862640_ForwardArgThroughFieldAndReturn, "auto");
