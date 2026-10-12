#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, HudStringShow_SetText, 0x004F3EF0,
         (void* self, void* edx, void* a4, float a5, int a6));

static void __fastcall HudStringShow_SetNotificationText(
    void* self, void* /*edx*/, void* text, float duration)
{
    HudStringShow_SetText(CSF_AT(void*, self, 0x8), nullptr,
                          text, duration, -1);
}

HOOK(0x004F49C0, HudStringShow_SetNotificationText, "auto");
