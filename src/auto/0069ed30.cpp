#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0069EB70: __cdecl. The original pushes four dwords (a1, a2, ECX, a2), so
// the callee sees four stack arguments and the caller cleans 0x10 bytes.
// The packet's two-argument bind line does not match the pushes, so this
// follows the pushes. The callee returns a char (al).
CSF_FUNC(char, __cdecl, RtFSManager_ForwardArgsToEB70, 0x0069EB70,
         (int a1, int a2, int a3, int a4));

// 0x0069ED30: __stdcall, two stack arguments, callee-cleaned (ret 8).
// Forwards (a1, a2, ECX, a2). ECX is not a parameter of a __stdcall function,
// so the third value is passed as 0 (uninitialized register, treated as 0).
static int __stdcall RtFSManager_ForwardFilePairToCore(int* a1, int* a2)
{
    return RtFSManager_ForwardArgsToEB70(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a1)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a2)),
        0,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(a2)));
}

HOOK(0x0069ED30, RtFSManager_ForwardFilePairToCore, "auto");
