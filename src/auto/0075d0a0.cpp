#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0075CF20: __thiscall-style callee (ecx = this, one stack arg), ret 4.
CSF_FUNC(int, __fastcall, GetFacingSourceObject, 0x0075CF20, (void* self, void* edx, uint32_t* a2));

// Returns the facing angle (radians) stored at +0x14 of the object resolved by
// GetFacingSourceObject. The original forwards `this` in ecx and the stack argument unchanged.
static double __fastcall C_0086c288_GetFacingAngleRadians(C_0086c288* self, void* /*edx*/, uint32_t* a2)
{
    int facingObject = GetFacingSourceObject(self, nullptr, a2);
    void* facingPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(facingObject));
    float angle = CSF_AT(float, facingPtr, 0x14);
    return static_cast<double>(angle);
}

HOOK(0x75D0A0, C_0086c288_GetFacingAngleRadians, "auto");
