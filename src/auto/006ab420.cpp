#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006AB250, __cdecl(void* a1, int a2). Caller pushes ecx (this) first,
// then a2, so a1 = a2 (the byte pointer) and a2 = this.
CSF_FUNC(int, __cdecl, sub_6AB250, 0x006AB250, (void* a1, int a2));

// Original: 0x006AB420, __thiscall with one stack argument (ret 4).
static uint32_t* __fastcall C_0085ff4c_ForwardToGatheringHelper(C_0085ff4c* self, void* /*edx*/, uint8_t* a2)
{
    int result = sub_6AB250(a2, static_cast<int>(reinterpret_cast<intptr_t>(self)));
    return reinterpret_cast<uint32_t*>(static_cast<intptr_t>(result));
}

HOOK(0x006AB420, C_0085ff4c_ForwardToGatheringHelper, "auto");
