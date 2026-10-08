#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_726D40(int a1, int a2, int a3, uint32_t context, uint32_t flag)
// The original pushes five arguments (0x14 bytes) before this cdecl call,
// even though Hex-Rays only reports the three leading stack arguments.
CSF_FUNC(int, __cdecl, CopyConstructPointerRange_726D40, 0x726D40, (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_727220.
//
// The function is entered with three stack arguments and it consumes the
// incoming ecx register, which is modelled here as the __thiscall-style
// "self" parameter (__fastcall keeps the stack layout and the ret 12 of the
// original). It relays exactly:
//
//     CopyConstructPointerRange_726D40(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The fifth value is not simply zero: the original pushes ecx (prologue
// `push ecx`), then writes a single zero byte over that stack slot
// (`mov byte ptr [ebp-4], 0`) and finally reloads the whole dword
// (`mov eax, [ebp-4]`). Only the low byte is therefore cleared; the upper
// three bytes still hold the incoming ecx bits. The incoming ecx is also
// pushed unchanged as the fourth argument (it is only reloaded from
// [ebp+0x10] into ecx after that push).
static int __fastcall InvokeCoreDispatcher(void* self, void* /*edx*/, int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CopyConstructPointerRange_726D40(a1, a2, a3, context, flag);
}

HOOK(0x727220, InvokeCoreDispatcher, "auto");
