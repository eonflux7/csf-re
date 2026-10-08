// Reimplementation of sub_542AF0 (0x00542AF0) -- module menu_controls.
//
// Original (MSVC 7.1, x86):
//   int __stdcall sub_542AF0(int a1, int a2)
//   {
//       sub_55D830(a1, a2);          // callee pops 8 bytes; receives incoming ecx as "this"
//       return sub_428500(a1);       // __cdecl, caller cleans up (add esp, 4)
//   }
//
// The first helper is a __thiscall whose "this" is the ecx value the caller of this
// function supplied (the function itself never touches ecx), so it is modelled as a
// __fastcall free function and the incoming ecx is forwarded unchanged.

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0055D830 -- __thiscall helper (this, arg2, arg3); the callee pops the two
// stack arguments itself (ret 8).
CSF_FUNC(int, __fastcall, MenuControls_Configure, 0x55D830, (void* self, void* edx, int a2, int a3));

// 0x00428500 -- __cdecl query returning an integer result (no callee cleanup).
CSF_FUNC(int, __cdecl, MenuControls_QueryState, 0x428500, (int a1));

// 0x00542AF0
// Forwards the incoming "this" (ecx) plus both of its own stack arguments to the
// menu-controls configuration helper, discards that result, and returns the value
// of the subsequent state query performed on the first argument.
static int __fastcall MenuControls_ConfigureThenQueryState(void* self, void* /*edx*/, int a1, int a2)
{
    MenuControls_Configure(self, nullptr, a1, a2);
    return MenuControls_QueryState(a1);
}

HOOK(0x542AF0, MenuControls_ConfigureThenQueryState, "auto");
