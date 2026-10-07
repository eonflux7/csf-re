#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00860288_ForwardToSubobjectVirtualAt88(
    C_00860288* self, void* /*edx*/)
{
    void** vtable = reinterpret_cast<void**>(self->field_0030);
    void* subobject = reinterpret_cast<uint8_t*>(self) + 0x30;

    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        vtable[0x88 / 4])(subobject, nullptr);
}

HOOK(0x006B2F30, C_00860288_ForwardToSubobjectVirtualAt88, "auto");
