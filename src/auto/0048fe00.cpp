#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, RefreshState, 0x0048FA00, (void* self, void* edx));

static void __fastcall C_00833908_PropagateState(
    C_00833908* self, void* /*edx*/, int state)
{
    void* primary = reinterpret_cast<void*>(self->vGetField016c());
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, primary, 0)[0x3C / 4])(primary, nullptr, state);

    void* secondary = reinterpret_cast<void*>(self->field_0044);
    if (secondary)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, secondary, 0)[0x94 / 4])(secondary, nullptr, state);
    }

    void* tertiary = reinterpret_cast<void*>(self->field_0048);
    if (tertiary)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, tertiary, 0)[0x30 / 4])(tertiary, nullptr, state);
    }

    RefreshState(self, nullptr);
}

HOOK(0x0048FE00, C_00833908_PropagateState, "auto");
