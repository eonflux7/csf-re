#include "hook.h"
#include "globals.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, Hud_UpdateSubtitlePanel, 0x004F3370, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, Hud_UpdateThirdPanel, 0x004F4050, (void* self, void* edx, float a3));

static void __fastcall Hud_UpdateSharedPanel(void* self, void* /*edx*/, float deltaTime)
{
    Hud_UpdateSubtitlePanel(CSF_AT(void*, self, 0x4), nullptr, deltaTime);
    Hud_UpdateSubtitlePanel(CSF_AT(void*, self, 0x8), nullptr, deltaTime);
    Hud_UpdateThirdPanel(CSF_AT(void*, self, 0xC), nullptr, deltaTime);
}

HOOK(0x004F49F0, Hud_UpdateSharedPanel, "auto");
