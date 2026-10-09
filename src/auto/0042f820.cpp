#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));

// When the three flags at +0x23D/+0x127/+0x121 are set: renders the widget at +0x100 (vtable slot 2), then the
// widgets at +0xC, +0 and +4 of the array at +0x74, each at its own rect. (Hex-Rays' a2@<edi> is 0x42F4F0's spurious
// edi parameter, see there.)
static void __fastcall Hud_DrawWidgetTripletIfReady(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0x23D) || !CSF_AT(uint8_t, self, 0x127) || !CSF_AT(uint8_t, self, 0x121))
        return;

    if (void* w = CSF_AT(void*, self, 0x100))
        (*reinterpret_cast<void(__fastcall***)(void*, void*)>(w))[0x8 / 4](w, nullptr);

    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, CSF_AT(void*, self, 0x74), 0xC), nullptr);
    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, CSF_AT(void*, self, 0x74), 0x0), nullptr);
    Hud_RenderWidgetAtScaledRect(CSF_AT(void*, CSF_AT(void*, self, 0x74), 0x4), nullptr);
}

HOOK(0x0042F820, Hud_DrawWidgetTripletIfReady, "auto");
