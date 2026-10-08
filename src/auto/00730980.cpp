#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_730850(int a1, int a2, int a3, int context, int flag)
// The original pushes five arguments (0x14 bytes) before this cdecl call,
// even though Hex-Rays only reports the three leading ones.  The callee
// returns with `ret` (0 stack bytes popped), so it is __cdecl.
CSF_FUNC(int, __cdecl, CopyConstructPointerRange_730850, 0x730850, (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_730980 (size 39 bytes).
//
// Clone of the 0x719AE0 wrapper: it is entered with three stack arguments and
// consumes the incoming ecx register, modelled here as the __thiscall-style
// "self" parameter (using __fastcall keeps the stack layout and the original's
// `ret 12`).  It relays exactly:
//
//     CopyConstructPointerRange_730850(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The fifth value is not simply zero: the prologue pushes ecx (`push ecx`),
// writes a single zero byte over that stack slot (`mov byte ptr [ebp-4], 0`)
// and only afterwards reloads the whole dword (`mov eax, [ebp-4]`).  Only the
// low byte is cleared; the upper three bytes still hold the incoming ecx bits.
static int __fastcall ForwardToDispatchDeferred(void* self, void* /*edx*/, int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CopyConstructPointerRange_730850(a1, a2, a3, context, flag);
}

HOOK(0x730980, ForwardToDispatchDeferred, "auto");
