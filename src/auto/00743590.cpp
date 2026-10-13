#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x4E14E0: __fastcall method of the child object stored at this+0xD4.
// Reached via the tail jump with ecx = child object and the stack argument forwarded.
CSF_FUNC(void, __fastcall, C_0083e5e8_ForwardArgAddressToBase, 0x004E14E0, (C_0083e5e8* self, void* /*edx*/, int a2));

// 0x743590: forwards the stack argument to the child object at +0xD4 if one exists;
// otherwise does nothing. Pops its 4-byte argument (ret 4).
static void __fastcall NetCallbacks_ForwardToChildAtD4(void* self, void* /*edx*/, int a2)
{
    uint32_t pChild = CSF_AT(uint32_t, self, 0xD4);
    if (pChild == 0)
        return;                         // original returns without touching the child
    C_0083e5e8_ForwardArgAddressToBase(reinterpret_cast<C_0083e5e8*>(pChild), nullptr, a2);
}

HOOK(0x743590, NetCallbacks_ForwardToChildAtD4, "auto");
