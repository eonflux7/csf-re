#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CMissionScript_Reset, 0x0046A7A0, (CMissionScript* self, void* edx));

// Tail-jumps to CMissionScript_Reset with ecx = the pointer stored at +4 of the wrapper.
static int __fastcall Scene_ResetSubsystemC0(void* self, void* /*edx*/)
{
    return CMissionScript_Reset(CSF_AT(CMissionScript*, self, 0x4), nullptr);
}

HOOK(0x00444880, Scene_ResetSubsystemC0, "auto");
