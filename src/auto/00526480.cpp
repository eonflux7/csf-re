#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static int __fastcall C_00840ff0_ForwardStateToGlobal(C_00840ff0* self, void* /*edx*/)
{
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_70DDE0()));

    // Slot 0x70 on the global object, thiscall with no stack args.
    int stateResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, globalObj, 0)[0x70 / 4])(globalObj, nullptr);
    if (stateResult == 0)
        return stateResult;

    void* stateObj = reinterpret_cast<void*>(static_cast<uintptr_t>(stateResult));

    // Slot 0x18 on the state object: `call [eax+0x18]` with ecx still holding stateObj (this), one pushed int
    // (flag), no stack cleanup by the caller. Thiscall shape: ecx = stateObj, one stack argument, callee pops.
    if (self->field_0004 == 5)
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObj, 0)[0x18 / 4])(stateObj, nullptr, 1);

    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObj, 0)[0x18 / 4])(stateObj, nullptr, 0);
}

HOOK(0x00526480, C_00840ff0_ForwardStateToGlobal, "auto");
