#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// C runtime vsprintf (cdecl, 3 args). The original calls it with (buffer, format, va).
CSF_FUNC(int, __cdecl, CrtVsprintf, 0x007AFCAA, (char* buffer, const char* format, __builtin_va_list args));

// Unnamed in the packet; fills two floats (x and y scale factors) at the given pointer.
CSF_FUNC(unsigned int, __cdecl, Hud_ComputeScaleFactors, 0x0042DE60, (float* scaleXY));

// Formats a printf-style string into the overlay text widget, scales a 2-int anchor point
// by the HUD scale factors, moves the widget there, and returns the widget's slot-2 result.
// hud: HUD object; the widget pointer lives at hud+0x240 and is re-read before each use.
static int __cdecl Hud_FormatAndPlaceOverlayText(void* hud, int32_t* anchor, const char* format, ...)
{
    char buffer[2048];

    __builtin_va_list args;
    __builtin_va_start(args, format);
    CrtVsprintf(buffer, format, args);
    __builtin_va_end(args);

    // SetText(char*) through vtable slot 15 (+0x3C)
    void* textWidget = CSF_AT(void*, hud, 0x240);
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, textWidget, 0)[15])(textWidget, nullptr, buffer);

    // Copy the anchor point (x, y)
    int32_t position[2];
    position[0] = anchor[0];
    position[1] = anchor[1];

    float scale[2] = {0.0f, 0.0f};
    Hud_ComputeScaleFactors(scale);

    // Each int32 is converted to double (exact for all int32 values) and multiplied by the
    // float scale in double precision; the product is truncated toward zero through an int64
    // and the low 32 bits are stored, matching the original's dword store.
    position[0] = static_cast<int32_t>(static_cast<int64_t>(static_cast<double>(position[0]) * static_cast<double>(scale[0])));
    position[1] = static_cast<int32_t>(static_cast<int64_t>(static_cast<double>(position[1]) * static_cast<double>(scale[1])));

    // SetPosition(int32_t*) through vtable slot 20 (+0x50)
    void* positionWidget = CSF_AT(void*, hud, 0x240);
    ((void(__fastcall*)(void*, void*, int32_t*))CSF_AT(void**, positionWidget, 0)[20])(positionWidget, nullptr, position);

    // Final widget call through vtable slot 2 (+0x08); its result is the function's result.
    void* visibleWidget = CSF_AT(void*, hud, 0x240);
    return ((int(__fastcall*)(void*, void*))CSF_AT(void**, visibleWidget, 0)[2])(visibleWidget, nullptr);
}

HOOK(0x0042F970, Hud_FormatAndPlaceOverlayText, "auto");
