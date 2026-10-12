#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: sub_6DEB50 (__thiscall, 8 stack args, callee pops 32)
CSF_FUNC(void, __fastcall, sub_6DEB50, 0x006DEB50,
         (void* self, void* edx, unsigned int a2, char a3, int a4, int a5, int a6, int a7, int a8, int a9));

// Forwards a2 with a fixed default parameter block (0,0,0,0,0,-1.0f bits,-1).
static void __fastcall TextureLoader_CallWithDefaultParams(S_006de5b0* self, void* /*edx*/, unsigned int a2)
{
    const char defaultByte = 0;
    const int defaultZero = 0;
    const int negOneFloatBits = -1082130432; // 0xBF800000 (-1.0f)
    const int negOne = -1;

    sub_6DEB50(self, nullptr, a2, defaultByte, defaultZero, defaultZero, defaultZero,
               defaultZero, negOneFloatBits, negOne);
}

HOOK(0x006DED70, TextureLoader_CallWithDefaultParams, "auto");
