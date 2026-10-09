#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stack-argument-taking __fastcall wrapper for the original's __thiscall (ret 16).
// Callee of the original: 0x4283E0, __thiscall-style taking (self, a2 array of 4 dwords).
CSF_FUNC(char, __fastcall, HudOverlay_SubmitFourDwords, 0x004283E0, (void* self, void* edx, uint32_t* a2));

static char __fastcall Hud_ForwardQuadToOverlay(void* self, void* /*edx*/, uint32_t arg0, uint32_t a3, uint32_t a4, uint32_t a5)
{
    // Member pointer at this+0x2CC (index 179 of the pointer array).
    void* overlay = CSF_AT(void*, self, 0x2CC);
    char result = 0;
    if (overlay)
    {
        uint32_t a2[4];
        a2[0] = arg0;
        a2[2] = a4;
        a2[1] = a3;
        a2[3] = a5;
        return HudOverlay_SubmitFourDwords(overlay, nullptr, a2);
    }
    return result;
}

HOOK(0x0042FA10, Hud_ForwardQuadToOverlay, "auto");
