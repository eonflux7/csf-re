#include "hook.h"
#include "globals.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SetSubtitlePanelPosition, 0x004F1CE0, (void* self, void* edx, float x, float y));

static int __fastcall HudStringShow_SetGlobalSubtitlePanelPosition(
    void* self, void* /*edx*/, float x, float y)
{
    return SetSubtitlePanelPosition(CSF_AT(void*, self, 0x8), nullptr, x, y);
}

HOOK(0x004F4990, HudStringShow_SetGlobalSubtitlePanelPosition, "auto");
