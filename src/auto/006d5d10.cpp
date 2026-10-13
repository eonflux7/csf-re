#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x006D5C90 (__thiscall, no stack args, returns char).
CSF_FUNC(char, __fastcall, GfxScene_RefreshControllerState, 0x006D5C90, (void* self, void* edx));

static char __fastcall GfxScene_SetControllerState(S_006d5c90* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(a2);
    if (self->field_0098 != static_cast<uint32_t>(a2))
    {
        self->field_0098 = static_cast<uint32_t>(a2);
        return GfxScene_RefreshControllerState(self, nullptr);
    }
    return result;
}

HOOK(0x006D5D10, GfxScene_SetControllerState, "auto");
