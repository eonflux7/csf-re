#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Decrements the float timer at +0xAC by elapsed time a2. When it goes below zero,
// clears the timer and invokes the owner's virtual slot 77 (+0x134) with (owner field +0x64, 0).
static void __fastcall CharActionFsm_DecrementTimerAndFire(void* self, void* /*edx*/, float elapsedTime)
{
    CSF_GLOBAL(float, g_fZero, 0x82D4CC);

    float remainingTime = CSF_AT(float, self, 0xAC) - elapsedTime;
    CSF_AT(float, self, 0xAC) = remainingTime;

    if (remainingTime < g_fZero)
    {
        int actionParam = CSF_AT(int, self, 0x64);
        CSF_AT(float, self, 0xAC) = 0.0f;
        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[0x134 / 4])(self, nullptr, actionParam, 0);
    }
}

HOOK(0x482670, CharActionFsm_DecrementTimerAndFire, "auto");
