#include <cstdint>
#include "globals.h"
#include "hook.h"

// CCamera::SetSubtitlePanelPosition (0x004F1CE0)
// Stores the two stack arguments at this+0x28 and this+0x2C (thiscall: ecx = this)
// and returns the first argument.
static int __fastcall CCamera_SetSubtitlePanelPosition(void* self, void* /*edx*/, int subtitleX, int subtitleY) {
    CSF_AT(int, self, 0x28) = subtitleX;
    CSF_AT(int, self, 0x2C) = subtitleY;
    return subtitleX;
}

HOOK(0x004F1CE0, CCamera_SetSubtitlePanelPosition, "auto");

