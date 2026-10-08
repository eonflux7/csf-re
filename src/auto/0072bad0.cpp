#include <cstdint>
#include "globals.h"
#include "hook.h"

// int __cdecl sub_72A660(int a1, int a2, int a3, int context, int flag)
// The original pushes five arguments (0x14 bytes) before this cdecl call,
// even though Hex-Rays only reports the three leading ones.
CSF_FUNC(int, __cdecl, CopyConstructPointerRange_72A660, 0x72A660,
         (int a1, int a2, int a3, uint32_t context, uint32_t flag));

// Reimplementation of sub_72BAD0.
//
// The function is entered with three stack arguments and it consumes the
// incoming ecx register, which is modelled here as the __thiscall-style
// "self" parameter (__fastcall keeps the stack layout and the ret 12 of the
// original). It relays exactly:
//
//     CopyConstructPointerRange_72A660(a1, a2, a3, ecx, ecx & 0xFFFFFF00)
//
// The fifth value is not simply zero: the original pushes ecx (prologue
// `push ecx`), then writes a single zero byte over that stack slot
// (`mov byte ptr [ebp-4], 0`) and finally reloads the whole dword
// (`mov eax, [ebp-4]`). Only the low byte is therefore cleared; the upper
// three bytes still hold the incoming ecx bits.
static int __fastcall ForwardContextAndClearedFlag(void* self, void* /*edx*/,
                                                   int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    const uint32_t flag = context & 0xFFFFFF00u;
    return CopyConstructPointerRange_72A660(a1, a2, a3, context, flag);
}

HOOK(0x72BAD0, ForwardContextAndClearedFlag, "auto");
