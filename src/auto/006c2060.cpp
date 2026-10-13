#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns 1 when the animation's type word (***this) differs from 0x111, else 0.
static int __fastcall GfxHAnim_IsSpecialAnimation(void*** self, void* /*edx*/) {
    void** outer = *self;                       // eax = *this
    void* middle = outer[0];                    // ecx = **this
    int animationType = CSF_AT(int, middle, 0); // edx = ***this
    return animationType != 0x111 ? 1 : 0;
}

HOOK(0x006C2060, GfxHAnim_IsSpecialAnimation, "auto");
