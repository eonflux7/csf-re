#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void, __fastcall, AnimCore_PrepareStep, 0x004FC050, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GlobalValue4FBDE0_SetToggleMode, 0x004FC210, (void* self, void* edx, int a3));

// Advances a timed motion: counts the timer down by a2; while it stays positive,
// moves the position value by -a2 * speed and forwards the integer position to the
// target object's vtable slot 7. When the timer runs out, turns the toggle mode off.
static void __fastcall Anim_AdvanceTimedMotion(void* self, void* /*edx*/, float a2)
{
    if (CSF_AT(int, self, 0x7C) == 0)
        return;

    AnimCore_PrepareStep(self, nullptr);

    if (!(CSF_AT(float, self, 0x80) > g_fZero))
        return;

    float remainingTime = CSF_AT(float, self, 0x80) - a2;
    CSF_AT(float, self, 0x80) = remainingTime;

    if (remainingTime > g_fZero)
    {
        float position = CSF_AT(float, self, 0x84) - a2 * CSF_AT(float, self, 0x88);
        CSF_AT(float, self, 0x84) = position;

        void* target = CSF_AT(void*, self, 0x5C);
        int positionInt = static_cast<int>(position);
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, target, 0)[7])(target, nullptr, 0, positionInt);
    }
    else
    {
        GlobalValue4FBDE0_SetToggleMode(self, nullptr, 0);
    }
}

HOOK(0x004FC6C0, Anim_AdvanceTimedMotion, "auto");
