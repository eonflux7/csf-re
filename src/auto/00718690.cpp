#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiControl_DispatchStateDependentAction(
    CGuiControl* self, void* /*edx*/, void* action)
{
    if (!action)
        return;

    if (((std::uint8_t(__fastcall*)(CGuiControl*, void*))
             self->vtbl[0x6C / 4])(self, nullptr) ||
        self->field_0015 == 0)
    {
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, action, 0)[0x08 / 4])(action, nullptr);
    }
    else
    {
        ((void(__fastcall*)(void*, void*))
             CSF_AT(void**, action, 0)[0x0C / 4])(action, nullptr);
    }
}

HOOK(0x00718690, CGuiControl_DispatchStateDependentAction, "auto");
