#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x30 has no typed wrapper, so it is called through the vtable.
// Its callee takes one int argument after `this` (push 1 before the call).
static char __fastcall C_00840ff0_UpdateIfFlagged(C_00840ff0* self, void* /*edx*/, int /*a2*/)
{
    self->vLoadSoundEntryFromManager();

    char flagIsSet = static_cast<char>(self->field_005d);
    if (flagIsSet)
    {
        ((void(__fastcall*)(void*, void*, int))self->vtbl[0x30 / 4])(self, nullptr, 1);
        return self->vSubmitSoundRequest(0, 0);
    }
    return flagIsSet;
}

HOOK(0x5265B0, C_00840ff0_UpdateIfFlagged, "auto");
