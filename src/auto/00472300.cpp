#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(int, __cdecl, DispatchWithContext, 0x00471B70,
                (int a1, int a2, int a3, int a4, int a5));

static int __fastcall ForwardThreeArgumentsWithContext(
    void* self, void* /*edx*/, int a1, int a2, int a3)
{
    const uint32_t context = reinterpret_cast<uint32_t>(self);
    return DispatchWithContext(a1, a2, a3, static_cast<int>(context),
                               static_cast<int>(context & 0xFFFFFF00u));
}

HOOK(0x00472300, ForwardThreeArgumentsWithContext, "auto");
