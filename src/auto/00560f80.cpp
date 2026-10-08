#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

static int __fastcall CMenuMpSessions_SetControlsActive(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* control = reinterpret_cast<void*>(self->field_00a4);
    if (control)
    {
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 1);
    }

    control = reinterpret_cast<void*>(self->field_0070);
    if (control)
    {
        ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 1);
    }

    control = reinterpret_cast<void*>(self->field_0034);
    if (control)
    {
        return ((int (__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[0xa8 / 4])(control, nullptr, 1);
    }

    return 0;
}

HOOK(0x00560F80, CMenuMpSessions_SetControlsActive, "auto");
