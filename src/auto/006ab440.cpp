#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006AB2E0, __cdecl(void* a1, int a2). Caller pushes this (ecx) and a2
// on the stack, so a1 = a2 (the object pointer argument) and a2 = this.
CSF_FUNC(uint32_t*, __cdecl, sub_6AB2E0, 0x006AB2E0, (void* a1, int a2));

// Original: 0x006AB440, __thiscall with one stack argument (ret 4).
static uint32_t* __fastcall C_0085ff4c_ForwardToGatheringDdlHelper(C_0085ff4c* self, void* /*edx*/, uint32_t* a2)
{
    return sub_6AB2E0(a2, static_cast<int>(reinterpret_cast<intptr_t>(self)));
}

HOOK(0x006AB440, C_0085ff4c_ForwardToGatheringDdlHelper, "auto");
