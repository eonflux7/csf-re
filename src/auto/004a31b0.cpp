#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Scene object query: returns nonzero when the scene object is inactive.
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));

// __thiscall method of C_00833b88. `self` points at a slot pair inside the
// owner: self[1] is the scene object, and self[-1] (self - 1) is the owner
// object whose callbacks are dispatched on.
static int __fastcall C_00833b88_DispatchInactiveOrActive(C_00833b88** self, void* /*edx*/, int a2)
{
    (void)a2; // the original pushes the constant 10, not a2; a2 is only popped (ret 4)

    const bool isInactive = SceneObj_IsInactive(self[1], nullptr);
    C_00833b88** owner = self - 1;

    // The original reloads *owner after the query call.
    void* ownerObj = *owner;

    if (isInactive)
    {
        // call [owner + 0x58] with ecx = owner, pushed argument 10
        auto handler = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void*, ownerObj, 0x58));
        return handler(owner, nullptr, 10);
    }

    // call [owner + 0x84] with ecx = owner
    auto handler = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void*, ownerObj, 0x84));
    return handler(owner, nullptr);
}

HOOK(0x4A31B0, C_00833b88_DispatchInactiveOrActive, "auto");
