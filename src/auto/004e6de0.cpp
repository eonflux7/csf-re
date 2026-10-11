#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards an integer argument to vtable slot 0xAC (43) of the object held in field_0008 and returns it unchanged.
static int __fastcall C_0083eb70_ForwardToSubobjectSlotAC(C_0083eb70* self, void* /*edx*/, int a2)
{
    void* subobject = reinterpret_cast<void*>(self->field_0008);
    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, subobject, 0)[0xAC / 4]))(subobject, nullptr, a2);
    return a2;
}

HOOK(0x004E6DE0, C_0083eb70_ForwardToSubobjectSlotAC, "auto");
