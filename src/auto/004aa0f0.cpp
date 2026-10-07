#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004AA0F0 (6 bytes): returns the constant string "TAtacar" (the attack action name).
// Called as a __thiscall method of TAtacar (no stack arguments, ret 0).
static const char* __fastcall TAtacar_GetAttackActionName(TAtacar* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00832E18, "TAtacar");
}

HOOK(0x004AA0F0, TAtacar_GetAttackActionName, "auto");
