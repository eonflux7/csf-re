#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_ResetChildAndDispatch(C_00833908* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0048);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, child, 0)[0x60 / 4])(child, nullptr);

    return self->vSlot8C(0, 0, -1, 0);
}

HOOK(0x0048E550, C_00833908_ResetChildAndDispatch, "auto");
