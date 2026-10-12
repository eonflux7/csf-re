#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, HudLayoutTextAndValue, 0x004F3EA0, (int subObject, void* edx, int a4, float a5, int a6));

static int __fastcall HudStringShow_LayoutTextAndValue(void* self, void* edx,
                                                      int text, float value, int options)
{
    return HudLayoutTextAndValue(CSF_AT(int, self, 0x4), edx, text, value, options);
}

HOOK(0x004F4980, HudStringShow_LayoutTextAndValue, "auto");
