#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Calls the virtual method in vtable slot 2 (offset +8), then sets the byte flag
// at +4 to 1 and returns 1.
static char __fastcall C_00845368_ActivateFlag(C_00845368* self, void* /*edx*/)
{
    ((void(__fastcall*)(void*, void*))self->vtbl[2])(self, nullptr);
    self->field_0004 = 1;
    return 1;
}

HOOK(0x0055BD30, C_00845368_ActivateFlag, "auto");
