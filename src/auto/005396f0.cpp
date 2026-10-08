#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __cdecl GetGlobalValue() -> 0x521BB0 (returns a global object/handle, no args)
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());

// void __thiscall sub_5231D0(int handle, int value) -> 0x5231D0 (ret 4)
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x5231D0, (int handle, void* edx, int value));

// void __cdecl nullsub_2() -> 0x506AC0 (no-op notification)
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());

// Stores the incoming value into the field at +0x11C, then forwards it to the global
// manager obtained via GetGlobalValue() and finally fires the notification no-op.
static void __fastcall CMenuLoadGame_SetField011CAndNotify(CMenuLoadGame* self, void* /*edx*/, int a2)
{
    self->field_011c = (uint32_t)a2;

    int handle = GetGlobalValue();
    GlobalSetValue_5231D0(handle, nullptr, a2);

    Nullsub_506AC0();
}

HOOK(0x5396F0, CMenuLoadGame_SetField011CAndNotify, "auto");
