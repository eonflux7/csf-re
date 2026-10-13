#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, C_0083b668_CanControlEntryBeUsed, 0x004D01F0, (C_0083b668* self, void* /*edx*/, int controlIndex));

// Original: push 0; call C_0083b668_CanControlEntryBeUsed; ret (callee pops nothing).
static bool __fastcall C_00835228_CanUseControlEntry0(C_00835228* self, void* /*edx*/)
{
    return C_0083b668_CanControlEntryBeUsed(reinterpret_cast<C_0083b668*>(self), nullptr, 0);
}

HOOK(0x004A8E20, C_00835228_CanUseControlEntry0, "auto");
