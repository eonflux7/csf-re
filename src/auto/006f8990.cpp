#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006F8810: __cdecl, reads only its first two stack arguments (begin, end).
CSF_FUNC(int, __cdecl, GfxScreenEffects_ResetEntryRange, 0x006F8810, (int begin, int end));

// 0x006F8990: __stdcall forwarder with two stack arguments (ret 8). It pushes
// the two arguments in the order (arg1, ECX, arg1, arg0) before calling the
// __cdecl reset routine. The callee only reads its first two parameters, which
// are arg0 and arg1, so the remaining pushed values are never read and are not
// reproduced. The four-slot cleanup is done by the callee's cdecl epilogue in
// the original, so this wrapper just returns the callee's result.
static int __stdcall GfxScreenEffects_ForwardResetEntryRange(int begin, int end)
{
    return GfxScreenEffects_ResetEntryRange(begin, end);
}

HOOK(0x006F8990, GfxScreenEffects_ForwardResetEntryRange, "auto");
