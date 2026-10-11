#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_6E9030, 0x006E9030, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Reads a raw dword stored at address `addr` plus `off` (used for untyped object links).
static int ReadDwordAtAddress(int addr, int off)
{
    return CSF_AT(int, reinterpret_cast<void*>(static_cast<uintptr_t>(addr)), off);
}

static double __fastcall C_00861e98_GetResolvedKeyValue(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(int, self, 0x1c) == 0)
        return 0.0;

    int nodeAddr = sub_6E9030(self, nullptr, a2);
    void* node = reinterpret_cast<void*>(static_cast<uintptr_t>(nodeAddr));

    int target;
    if (CSF_AT(float, node, 8) <= g_fZero)
    {
        // Node's vtable slot 8 (offset 0x20) yields the target.
        int vtable = CSF_AT(int, node, 0);
        target = ReadDwordAtAddress(vtable, 32);
    }
    else
    {
        // Follow node+4 -> +12 -> slot 8 (offset 0x20).
        int inner = CSF_AT(int, node, 4);
        int mid = ReadDwordAtAddress(inner, 12);
        target = ReadDwordAtAddress(mid, 32);
    }

    if (target != 0 && ReadDwordAtAddress(target, 0) != 0)
    {
        return CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(target)), 4);
    }
    return 0.0;
}

HOOK(0x006E97C0, C_00861e98_GetResolvedKeyValue, "auto");
