#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Calls the virtual method at vtable slot 0x58/4 (a no-argument __thiscall
// method taking `this`), then returns the dword at +0x24.
static int __fastcall C_00861d48_UpdateThenGetField24(C_00861d48* self, void* /*edx*/)
{
    ((void(__fastcall*)(C_00861d48*, void*))self->vtbl[0x58 / 4])(self, nullptr);
    return static_cast<int>(self->field_0024);
}

HOOK(0x006E6B60, C_00861d48_UpdateThenGetField24, "auto");
