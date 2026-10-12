#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: __fastcall (this in ecx, edx unused), 25 stack dwords (a2..a26), ret 100.
CSF_FUNC(int, __fastcall, AnimationDefs_DispatchAnimation, 0x00774070,
         (void* self, void* edx, unsigned int a2, char a3, int a4, int a5, int a6, int a7, int a8, int a9,
          int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19,
          int a20, int a21, int a22, int a23, int a24, int a25, int a26));

// Calls the dispatcher with the fixed default animation parameter block the original builds on its stack.
// Stack slots a4..a26 that the original never writes are passed as 0 (uninitialized values are garbage).
static int __fastcall AnimationDefs_CallWithDefaultParams(S_00773d00* self, void* /*edx*/, unsigned int a2)
{
    return AnimationDefs_DispatchAnimation(self, nullptr, a2,
        -1,         // a3  (byte-sized, stored as dword -1 at block offset 0)
        0, 0, 0, 0, 0, 0,   // a4..a9
        15,                 // a10 (block offset 0x1C)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a11..a22
        15,                 // a23 (block offset 0x50)
        1,                  // a24 (block offset 0x54)
        0x1000000,          // a25 (block offset 0x58: byte 0x5B = 1)
        0);                 // a26 (block offset 0x5C)
}

HOOK(0x00774160, AnimationDefs_CallWithDefaultParams, "auto");
