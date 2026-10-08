#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_533A70(int a1, int a2, int a3, int ecx_context, uint32_t ecx_low_cleared)
//
// The original call site pushes five dwords (0x14 bytes) even though the
// Hex-Rays prototype only lists three parameters.  The trailing two values
// come from the incoming ecx register (context) and from the same register
// with its low byte cleared (flag).
CSF_FUNC(int, __cdecl, CopyDwordRange_533A70, 0x533A70, (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_533E50.
//
// The function is entered with three stack arguments and consumes the
// incoming ecx register, which is modelled as the __thiscall-style "self"
// parameter (__fastcall keeps ecx/edx in registers and 3 dwords on the
// stack, so the original `ret 12` is reproduced exactly).  It relays:
//
//     CopyDwordRange_533A70(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The fifth value is not a plain zero: the prologue reserves a slot with
// `push ecx`, then clears only its low byte (`mov byte ptr [ebp-4], 0`) and
// finally reloads the whole dword (`mov eax, [ebp-4]`).  The upper three
// bytes therefore still carry the incoming ecx bits.
static int __fastcall ForwardToMenuHandler(void* self, void* /*edx*/, int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CopyDwordRange_533A70(a1, a2, a3, context, flag);
}

HOOK(0x533E50, ForwardToMenuHandler, "auto");
