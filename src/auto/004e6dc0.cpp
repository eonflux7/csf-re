#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards an integer argument to vtable slot 0xA4 (index 41) of the child object held in
// field_0008 and returns it unchanged. The slot takes one int argument, so the typed
// vSlotA4() wrapper (no arguments) does not apply; the raw slot call is used instead.
static int __fastcall C_0083eb70_ForwardToChildSlotA4AndReturnArg(C_0083eb70* self, void* /*edx*/, int a2)
{
    void* child = reinterpret_cast<void*>(self->field_0008);
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, child, 0)[0xA4 / 4]))(child, nullptr, a2);
    return a2;
}

HOOK(0x004E6DC0, C_0083eb70_ForwardToChildSlotA4AndReturnArg, "auto");
