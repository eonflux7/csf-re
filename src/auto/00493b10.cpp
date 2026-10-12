#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original forwarder: C_00833908::ForwardValueAndRefresh (0x48FE50), __thiscall, one stack arg, ret 4.
CSF_FUNC(void, __fastcall, C_00833908_ForwardValueAndRefresh, 0x0048FE50, (C_00833908* self, void* /*edx*/, int value));

// Hook for 0x493B10 (CPlayerCharacter, __thiscall, one stack arg, ret 4).
// Forwards the argument to the base refresh, then calls virtual slot +0x98 (38) on the
// object held at field_0044 with the same argument and returns that int result.
// Typed as the base C_00833908 (the field offsets are the same through the derived class).
static int __fastcall CPlayerCharacter_ForwardValueAndDelegate(C_00833908* self, void* /*edx*/, int value)
{
    C_00833908_ForwardValueAndRefresh(self, nullptr, value);

    void* delegate = reinterpret_cast<void*>(self->field_0044);
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, delegate, 0)[0x98 / 4])(delegate, nullptr, value);
}

HOOK(0x00493B10, CPlayerCharacter_ForwardValueAndDelegate, "auto");
