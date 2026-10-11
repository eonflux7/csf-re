#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class routine that the original tail-calls with `this` in ecx (no stack args).
CSF_FUNC(int, __fastcall, C_0083e160_RefreshState, 0x004E1320, (void* self, void* edx));

// Clears the cached flag, notifies the child object through vtable slot 0x58
// with a three-dword descriptor { vtable 0x82D448, -1, -1 }, then runs the base refresh.
static int __fastcall C_0083e6d8_ClearFlagAndNotifyChild(C_0083e6d8* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_000c);

    uint32_t descriptor[3];
    descriptor[0] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    descriptor[1] = 0xFFFFFFFFu;
    descriptor[2] = 0xFFFFFFFFu;

    self->field_0008 = 0;

    // Child's vtable slot 0x58 / 4 = 22 (object type unknown here).
    ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, child, 0)[22])(
        child, nullptr, reinterpret_cast<void*>(descriptor));

    return C_0083e160_RefreshState(self, nullptr);
}

HOOK(0x004E2C80, C_0083e6d8_ClearFlagAndNotifyChild, "auto");
