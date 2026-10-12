#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, sub_4F24E0, 0x004F24E0,
         (int a1, int a2, int a3, int a4, int a5));

static int __fastcall HudStringShow_ForwardWithContext(
    void* self, void* /*edx*/, int a1, int a2, int a3)
{
    return sub_4F24E0(a1, a2, a3,
                     static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
                     a3);
}

HOOK(0x004F2EA0, HudStringShow_ForwardWithContext, "auto");
