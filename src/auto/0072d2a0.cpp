#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original: fld dword ptr [ecx+0x138] ; ret
// Loads the single-precision field at +0x138 and returns it in st(0)
// (extended precision, as Hex-Rays reports a `double` return type).
static double __fastcall CGuiSlider_GetField0138(CGuiSlider* self, void* /*edx*/)
{
    return static_cast<double>(self->field_0138);
}

HOOK(0x0072D2A0, CGuiSlider_GetField0138, "auto");
