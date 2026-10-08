// Reimplementation of sub_542AD0 (0x00542AD0) -- module menu_controls.
//
// Original (MSVC 7.1, x86):
//   int __stdcall sub_542AD0(int a1, int a2)
//   {
//       sub_542F70(a1, a2);       // callee pops 8 bytes; receives the incoming ecx as "this"
//       return sub_428480(a1);    // __cdecl, caller cleans up (add esp, 4)
//   }
//
// The disassembly never writes ecx before the first call, so the first helper is a
// __thiscall whose "this" is the ecx value the caller of this function supplied. The
// function is therefore modelled as a __fastcall free function (ecx = this, edx unused)
// and that incoming ecx is forwarded unchanged to the helper.

#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00542F70 -- __thiscall helper (this, arg2, arg3); the callee pops both stack
// arguments itself (ret 8). Return value is discarded by the caller.
CSF_FUNC(int, __fastcall, MenuControls_StoreParams, 0x542F70, (void* self, void* edx, int a2, int a3));

// 0x00428480 -- __cdecl query returning an integer result (no callee cleanup; the
// caller does `add esp, 4`).
CSF_FUNC(int, __cdecl, MenuControls_QueryParam, 0x428480, (int a1));

// 0x00542AD0
// Forwards the incoming "this" (ecx) plus both of its own stack arguments to the
// menu-controls parameter helper, discards that helper's result, and returns the value
// of the subsequent query performed on the first stack argument.
static int __fastcall MenuControls_StoreParamsThenQueryParam(void* self, void* /*edx*/, int a1, int a2)
{
    MenuControls_StoreParams(self, nullptr, a1, a2);
    return MenuControls_QueryParam(a1);
}

HOOK(0x542AD0, MenuControls_StoreParamsThenQueryParam, "auto");
