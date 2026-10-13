#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards an event value to the child effect stored at +0x08.
// The child's virtual slot 0x64 receives the value; the same value is returned.
// If there is no child effect, the value is returned unchanged.
static int __fastcall GuiEffect_ForwardToChildAndReturnArg(C_008656d0* self, void* /*edx*/, int a2)
{
    uint32_t childEffect = self->field_0008;
    if (childEffect)
    {
        reinterpret_cast<C_00863f60*>(childEffect)->vSlot64(a2);
    }
    return a2;
}

HOOK(0x713DD0, GuiEffect_ForwardToChildAndReturnArg, "auto");
