#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, sub_478880, 0x00478880, (int a1));

static int __fastcall C_0086754c_GetItemIdentifier(
    C_0086754c* self, void* /*edx*/)
{
    const int type =
        reinterpret_cast<int (__fastcall*)(C_0086754c*, void*)>(
            CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr);
    if (type != 3)
        return -1;

    const int identifier =
        CSF_AT(int, CSF_AT(void*, self, 0x40), 0x74);
    if (sub_478880(identifier) == -1)
        return -1;

    if (identifier == 1 || identifier == 21)
        return 6;
    return identifier;
}

HOOK(0x00479310, C_0086754c_GetItemIdentifier, "auto");
