#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, C_008656d0_SetOrClearFlagBits, 0x006C5BE0, (C_008656d0* self, void* /*edx*/, int bitMask, char enable));

// Sets or clears flag bit 0x400 on the global object returned by GetGlobalValue_6C5760.
static int __stdcall GfxScene_SetOrClearGlobalFlag400(char enable)
{
    C_008656d0* flagsObject = reinterpret_cast<C_008656d0*>(GetGlobalValue_6C5760());
    return C_008656d0_SetOrClearFlagBits(flagsObject, nullptr, 0x400, enable);
}

HOOK(0x006CAC20, GfxScene_SetOrClearGlobalFlag400, "auto");
