#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __stdcall, sub_753410, 0x00753410, (int a1));
CSF_FUNC(char, __stdcall, sub_65E740, 0x0065E740, (int a1, int a2));

// Routes a message argument to sub_753410 when it matches one of the member
// slots at this+0x44, +0x6C, +0x6F or +0x72 (or is null); otherwise forwards
// to sub_65E740.
static int __fastcall NetCogibleDdl_RouteToMemberSlot(void* self, void* /*edx*/, char* a2, int a3)
{
    char* base = static_cast<char*>(self);

    if (!a2)
        sub_753410(a3);
    if (a2 == base + 0x44)
        return sub_753410(a3);

    if (!a2)
        sub_753410(a3);
    if (a2 == base + 0x6C)
        return sub_753410(a3);

    if (!a2)
        sub_753410(a3);
    if (a2 == base + 0x6F)
        return sub_753410(a3);

    if (!a2)
        sub_753410(a3);
    if (a2 == base + 0x72)
        return sub_753410(a3);

    return sub_65E740(reinterpret_cast<int>(a2), a3);
}

HOOK(0x00742080, NetCogibleDdl_RouteToMemberSlot, "auto");
