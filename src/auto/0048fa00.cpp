#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __fastcall C_00833908_UpdateTransitionFlag(C_00833908* self, void* /*edx*/)
{
    bool active = false;
    void* object = reinterpret_cast<void*>(self->vGetField016c());
    if (object)
    {
        active = ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x40 / 4])(object, nullptr);
        if (active)
        {
            void* transition = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x4C / 4])(object, nullptr);
            if (transition && self->field_0050 != 32)
            {
                double value = ((double(__fastcall*)(void*, void*))
                    CSF_AT(void**, transition, 0)[8 / 4])(transition, nullptr);
                active = value == g_TransitionOne;
            }
        }
    }

    if (active)
        self->field_0170 |= 4u;
    else
        self->field_0170 &= ~4u;
}

HOOK(0x0048FA00, C_00833908_UpdateTransitionFlag, "auto");
