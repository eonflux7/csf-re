#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004D0250 C_0083b668_IsEntryTargetable(this, a2) -- __thiscall, ret 4.
CSF_FUNC(bool, __fastcall, C_0083b668_IsEntryTargetable, 0x004D0250, (C_0083b668* self, void* /*edx*/, int a2));

// Original 0x004A8E30: pushes 0 and tail-calls-with-return the entry-targetable check
// on the base sub-object (same address as this), returning its bool result.
static bool __fastcall C_00835228_IsEntryTargetableDefault(C_00835228* self, void* /*edx*/)
{
    return C_0083b668_IsEntryTargetable(reinterpret_cast<C_0083b668*>(self), nullptr, 0);
}

HOOK(0x004A8E30, C_00835228_IsEntryTargetableDefault, "auto");
