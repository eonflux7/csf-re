#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// int __cdecl GetGlobalValue()  @ 0x521BB0
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());

// void __thiscall sub_5231D0(int this, int a2)  @ 0x5231D0  (pops 4 bytes)
CSF_FUNC(void, __fastcall, Sub5231D0, 0x5231D0, (int this_, void* /*edx*/, int a2));

// void nullsub_2()  @ 0x506AC0  (pops 0 bytes)
CSF_FUNC(void, __cdecl, Nullsub506AC0, 0x506AC0, ());

// int __usercall sub_542F10(int* a1@<ecx>, int a2@<ebx>)  @ 0x542F10  (pops 0 bytes)
CSF_FUNC(int, __fastcall, Sub542F10, 0x542F10, (int* a1, int a2));

// 0x00543120
// CMenuControls (ListboxConfiguracion / BotonMovement / BotonLook / BotonAttack /
// BotonSave / BotonReset) method: writes the incoming value into the screen's
// field at +0x28, runs the global-value notifier on it, then refreshes the screen.
// ABI: this in ecx, a2 in ebx, a3 on the stack; callee pops 4 bytes (ret 4).
static int __fastcall CMenuControls_SetValueAndNotify(void* self, int a2, int a3)
{
    CSF_AT(uint32_t, self, 0x28) = static_cast<uint32_t>(a3);

    int globalValue = GetGlobalValue();
    Sub5231D0(globalValue, nullptr, a3);

    Nullsub506AC0();

    return Sub542F10(reinterpret_cast<int*>(self), a2);
}

HOOK(0x543120, CMenuControls_SetValueAndNotify, "auto");
