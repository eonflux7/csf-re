// Reimplementation of 0x0073C520 (12 bytes, region game).
//
//   call 0x648BE0        ; fetch an object pointer (no arguments)
//   mov  ecx, eax        ; use it as the implicit `this`
//   jmp  0x648E70        ; tail-call a __thiscall method on that object
//
// Neither callee touches the incoming stack, and the tail jump forwards the
// return value of 0x648E70 straight back to our caller, so a plain
// call + return is functionally identical (both pop 0 stack bytes).

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x648BE0: no-argument accessor that returns the target object pointer in eax.
CSF_FUNC(void*, __cdecl, FetchObject, 0x648BE0, ());

// 0x648E70: __thiscall method (object passed in ecx, ret 0).
CSF_FUNC(int, __fastcall, InvokeOnObject, 0x648E70, (void* self, void* /*edx*/));

static int __cdecl InvokeMethodOnFetchedObject()
{
    void* object = FetchObject();
    return InvokeOnObject(object, nullptr);
}

HOOK(0x73C520, InvokeMethodOnFetchedObject, "auto");
