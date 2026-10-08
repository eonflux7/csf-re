// Reimplementation of sub_542B10 (0x00542B10) -- module menu_controls.
//
// Original (MSVC 7.1, x86):
//   int __stdcall sub_542B10(int a1, int a2)
//   {
//       sub_542E20(this, a1, a2);    // __thiscall: ecx is untouched by this
//                                    // function, so the caller's ecx is
//                                    // forwarded; callee pops 8 bytes.
//       return sub_4284C0(a1);       // __cdecl, caller cleans up (add esp, 4)
//   }
//
// The function itself never writes ecx: the caller hands the "this" pointer to
// the first callee exactly as received. It is therefore modelled as a
// __fastcall free function whose incoming ecx is forwarded unchanged (the same
// shape as the verified sibling 0x00542AF0).

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00542E20 -- __thiscall helper (this, a2, a3); the callee pops the two stack
// arguments itself (ret 8).
CSF_FUNC(void, __fastcall, CMenuControls_ResetStateIfThree, 0x542E20, (void* self, void* edx, int a2, int a3));

// 0x004284C0 -- __cdecl query returning an integer result (no callee cleanup).
CSF_FUNC(int, __cdecl, MenuOptions_ProcessValue_4284C0, 0x4284C0, (int a1));

// 0x00542B10
// Forwards the incoming "this" (ecx) plus both of its own stack arguments to the
// menu-controls reset helper (which only resets when the state equals three),
// discards that result, and returns the value of the subsequent query performed
// on the first argument.
static int __fastcall MenuControls_ResetStateIfThreeThenQueryState(void* self, void* /*edx*/, int menuArg, int modeArg)
{
    CMenuControls_ResetStateIfThree(self, nullptr, menuArg, modeArg);
    return MenuOptions_ProcessValue_4284C0(menuArg);
}

HOOK(0x542B10, MenuControls_ResetStateIfThreeThenQueryState, "auto");
