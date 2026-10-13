#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* , int));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Resets the transition/fade state block owned by this object. Only runs when
// field_0020 (a child object) is present.
static void __fastcall C_00862390_ResetTransitionState(C_00862390* self, void* /*edx*/, int a2)
{
    if (self->field_0020)
    {
        C_00860a30_ClearField000c(reinterpret_cast<C_00860a30*>(self), nullptr, a2);

        // Child object virtual slot 0x2C: SetValue3d(1)
        void* child = reinterpret_cast<void*>(self->field_0020);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x2C / 4])(child, nullptr, 1);

        float* sourceValue = reinterpret_cast<float*>(self->field_0018 + 0x58);
        if (*sourceValue == g_fZero)
        {
            self->field_0024 = 1.0f;
        }
        else
        {
            self->field_0024 = 0.0f;
            float sourceCopy = *reinterpret_cast<float*>(self->field_0018 + 0x58);
            self->field_002c = *reinterpret_cast<float*>(self->field_0018 + 0x58);
            self->field_0028 = g_TransitionOne / sourceCopy;
        }

        void* lockObj = reinterpret_cast<void*>(self->field_0034);
        if (lockObj)
        {
            // slot 0x1C: no-arg call
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, lockObj, 0)[0x1C / 4])(lockObj, nullptr);

            void* obj = reinterpret_cast<void*>(self->field_0034);
            a2 = 0;
            // slot 0x18: (&a2, 0)
            ((void(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, obj, 0)[0x18 / 4])(obj, nullptr, &a2, 0);
            self->field_0038 = 0.0f;
        }

        self->field_0030 = 0.0f;
    }
}

HOOK(0x006F0D40, C_00862390_ResetTransitionState, "auto");
