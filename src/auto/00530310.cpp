#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CMenuMpServerInfo::ActivateChild
// Sets the byte flag at +0x1F4 to 1 and forwards (this, 1) to the virtual method
// living at vtable slot 24 (byte offset 0x60) of the child object stored at +0x1E8.
// The virtual callee is a __thiscall (ecx = child, stack args = this, 1) and pops
// its 8 stack bytes itself; it is reproduced with a __fastcall pointer so the
// register/stack layout is identical.
static int __fastcall CMenuMpServerInfo_ActivateChild(void* self, void* /*edx*/)
{
    void* child = CSF_AT(void*, self, 0x1e8);
    CSF_AT(uint8_t, self, 0x1f4) = 1;

    typedef int(__fastcall* ActivateChildFn)(void*, void*, void*, int);
    ActivateChildFn fn = reinterpret_cast<ActivateChildFn>(CSF_AT(void**, child, 0)[0x60 / 4]);
    return fn(child, nullptr, self, 1);
}

HOOK(0x00530310, CMenuMpServerInfo_ActivateChild, "auto");
