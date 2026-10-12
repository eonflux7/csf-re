#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, ForwardArgumentsWithContext, 0x00471B70,
         (int a1, int a2, int a3, int a4, int a5));

static int __fastcall ForwardWithSelfContext(void* self, void* /*edx*/,
                                             int a1, int a2, int a3)
{
    return ForwardArgumentsWithContext(
        a1, a2, a3, static_cast<int>(reinterpret_cast<std::uintptr_t>(self)), a3);
}

HOOK(0x00472330, ForwardWithSelfContext, "auto");
