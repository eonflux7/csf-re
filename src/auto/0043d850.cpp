#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Frees the owned buffer at +0x50, clears the three slots, then tail-jumps
// into the child object at +0x1C through its vtable slot 0x28.
static hook::Tail __fastcall C_00830e5c_ResetAndForwardToChild(void* self, const uint32_t* /*args*/)
{
    auto* owner = reinterpret_cast<C_00830e5c*>(self);

    if (owner->field_0050)
        Mem_FreeDebug(reinterpret_cast<void*>(owner->field_0050));
    owner->field_0050 = 0;
    owner->field_0054 = 0;
    owner->field_0058 = 0;

    void* child = reinterpret_cast<void*>(owner->field_001c);
    if (!child)
        return hook::tail_ret(0);

    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x28 / 4]);
}

HOOK_TAIL(0x0043D850, C_00830e5c_ResetAndForwardToChild, "minimap");
