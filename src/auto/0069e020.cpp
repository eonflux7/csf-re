#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee at 0x69DEF0: __fastcall-style, self in ecx, edx, then 3 stack args (ret 12).
CSF_FUNC(void*, __fastcall, Tokenizer_AllocInner, 0x0069DEF0,
         (void* self, void* edx, uint32_t Size, int a3, int a4));

// The original is a __thiscall with 4 stack args (ret 16). It never writes ecx,
// so ecx on the call is this function's own `this` on entry. It loads edx from
// [ebp+0x10] (a4) and leaves it set when calling the inner allocator.
static void* __fastcall C_0085eac0_ForwardAlloc(C_0085eac0* self, void* edx,
                                                size_t Size, int a3, int a4, int a5)
{
    // Original: push [ebp+14] (a5); mov edx,[ebp+10] (a4); push edx; push [ebp+8] (Size); call 0x69DEF0
    // ecx is still this (self); edx holds a4 at the call.
    (void)a3;
    (void)edx;
    return Tokenizer_AllocInner(self, reinterpret_cast<void*>(static_cast<uintptr_t>(a4)),
                                static_cast<uint32_t>(Size), a4, a5);
}

HOOK(0x69E020, C_0085eac0_ForwardAlloc, "auto");
