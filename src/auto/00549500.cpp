// Reimplementation of sub_549500 (0x00549500) -- module menu_video_options
// (CMenuVideoOptions / CMenuVideoAdvanced).
//
// Original (MSVC 7.1, x86):
//   int __stdcall sub_549500(int a1, int a2)
//   {
//       sub_55DE90(a1, a2);   // __thiscall helper; callee pops 8 bytes
//       return sub_428480(a1); // __cdecl, caller cleans up (add esp, 4)
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

// 0x0055DE90 -- __thiscall helper (this, a2, a3); the callee pops the two stack
// arguments itself (ret 8).
CSF_FUNC(int, __fastcall, CMenuAccountEdit_HandleEmbeddedControlEvent, 0x55DE90,
         (void* self, void* edx, int a2, int a3));

// 0x00428480 -- __cdecl query returning an integer result (no callee cleanup).
CSF_FUNC(int, __cdecl, MenuVideoOptions_QueryState, 0x428480, (int a1));

// 0x00549500
// Forwards the incoming "this" (ecx) plus both of its own stack arguments to the
// embedded-control event handler, discards that result, and returns the value of the
// subsequent state query performed on the first argument.
static int __fastcall MenuVideoOptions_HandleEmbeddedControlEventThenQuery(
    void* self, void* /*edx*/, int a1, int a2)
{
    CMenuAccountEdit_HandleEmbeddedControlEvent(self, nullptr, a1, a2);
    return MenuVideoOptions_QueryState(a1);
}

HOOK(0x549500, MenuVideoOptions_HandleEmbeddedControlEventThenQuery, "auto");
