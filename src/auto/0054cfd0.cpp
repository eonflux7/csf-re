#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, sub_54C1D0, 0x0054C1D0,
         (int a1, int a2, int a3, int a4, int a5));

static int __fastcall TypeRegistry_ForwardWithContext(
    void* self, void* /*edx*/, int a1, int a2, int a3)
{
    return sub_54C1D0(a1, a2, a3,
                     static_cast<int>(reinterpret_cast<std::intptr_t>(self)),
                     a3);
}

HOOK(0x0054CFD0, TypeRegistry_ForwardWithContext, "auto");
