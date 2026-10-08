#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __cdecl GetGlobalValue_521BB0() -> 0x521BB0 (returns a global manager handle, no args)
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());

// void __thiscall sub_5231D0(int handle, int value) -> 0x5231D0 (ret 4)
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x5231D0, (int handle, void* edx, int value));

// void __cdecl nullsub_2() -> 0x506AC0 (no-op notification)
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());

// Stores the incoming value into field +0x50, forwards it to the global manager
// obtained via GetGlobalValue_521BB0(), then fires the notification no-op.
static void __fastcall CMenuGameOptions_SetValueAndNotify(CMenuGameOptions* self, void* /*edx*/, int a2)
{
    self->field_0050 = a2;

    int handle = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(handle, nullptr, a2);

    Nullsub_506AC0();
}

HOOK(0x53B2E0, CMenuGameOptions_SetValueAndNotify, "auto");
