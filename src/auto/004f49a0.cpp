#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, HudLayoutTextAndValue, 0x004F3EA0, (int subObject, void* edx, int a4, float a5, int a6));

static void __fastcall HudStringShow_SetGlobalNameWithValue(
    void* self, void* /*edx*/, int name, float value)
{
    HudLayoutTextAndValue(CSF_AT(int, self, 0x8), nullptr, name, value, -1);
}

HOOK(0x004F49A0, HudStringShow_SetGlobalNameWithValue, "auto");
