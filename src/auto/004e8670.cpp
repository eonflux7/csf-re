#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address with their real signatures.
CSF_FUNC(char, __fastcall, sub_4E53A0, 0x004E53A0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(bool, __fastcall, sub_4E7660, 0x004E7660, (void* self, void* edx, int a2));

// Runs the 0x4E53A0 check; if it succeeds, returns true unless a3 == 10 or the
// 0x4E7660 check passes. Returns the raw 0x4E53A0 byte on failure.
static char __fastcall C_0083eb70_CheckPassesUnlessTypeTen(C_0083eb70* self, void* /*edx*/, int a2, int a3)
{
    char result = sub_4E53A0(self, nullptr, a2, a3);
    if (result)
    {
        bool excluded = false;
        if (a3 != 10)
            excluded = sub_4E7660(self, nullptr, a2);
        return !excluded;
    }
    return result;
}

HOOK(0x004E8670, C_0083eb70_CheckPassesUnlessTypeTen, "auto");
