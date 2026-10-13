#include <cstdint>
#include "globals.h"
#include "hook.h"

// Tail-call targets (bound by address, signatures from the packet).
CSF_FUNC(char, __fastcall, HudPortraits_ContinueAnimation, 0x00457ED0, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, HudPortraits_ResetAnimation, 0x00457F30, (void* self, void* edx, float a3));

// Original: char __stdcall UpdateGlobalController(float a2), with `this` in ecx, so it is
// written as a __fastcall with one stack argument (ret 4).
static char __fastcall HudPortraits_UpdateGlobalController(void* self, void* /*edx*/, float a2)
{
    // Accumulate the elapsed time into field 0x20.
    CSF_AT(float, self, 0x20) = a2 + CSF_AT(float, self, 0x20);

    // Dispatch on the step counter at field 0x1c.
    uint32_t v3 = CSF_AT(uint32_t, self, 0x1c) - 1;
    if (v3 == 0) {
        // Tail jump: forward ecx and the float argument unchanged.
        return HudPortraits_ResetAnimation(self, nullptr, a2);
    }

    uint32_t v4 = v3 - 1;
    if (v4 == 0) {
        // Tail jump: forward ecx and the float argument unchanged.
        return HudPortraits_ContinueAnimation(self, nullptr, a2);
    }

    // Falls through: returns the low byte of the remaining counter value.
    return static_cast<char>(v4);
}

HOOK(0x004580C0, HudPortraits_UpdateGlobalController, "auto");
