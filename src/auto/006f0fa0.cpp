#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_6CA410, 0x006CA410, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Copies the source's float range fields into the target object held in
// this->field_0018, and clamps the target's +0x70 value into [0, 1].
static void __fastcall C_00862390_ApplyFloatLimits(C_00862390* self, void* /*edx*/, int a2)
{
    sub_6CA410(self, nullptr, a2);

    uint8_t* source = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    if (CSF_AT(float, source, 0x6C) > CSF_AT(float, source, 0x68))
    {
        uint8_t* target = reinterpret_cast<uint8_t*>(self->field_0018);
        CSF_AT(uint32_t, target, 0x6C) = CSF_AT(uint32_t, source, 0x68);
    }

    if (CSF_AT(float, source, 0x70) > g_TransitionOne || CSF_AT(float, source, 0x70) < g_fZero)
    {
        uint8_t* target = reinterpret_cast<uint8_t*>(self->field_0018);
        int lowerBoundBits = 0;
        a2 = 0x3F800000; // 1.0f, written to the stack argument slot as the original does
        ClampFloatToBounds(reinterpret_cast<float*>(target + 0x70),
                           reinterpret_cast<float*>(&lowerBoundBits),
                           reinterpret_cast<float*>(&a2));
    }
}

HOOK(0x006F0FA0, C_00862390_ApplyFloatLimits, "auto");
