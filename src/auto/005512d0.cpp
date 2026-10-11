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

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, sub_73C050, 0x0073C050, (void* self, void* edx, int arg0, int a2));

// Sends one spawn-point category (1, 2, 3) through sub_73C050, with the
// global dword fetched before each call. Callee pops 8 bytes (__fastcall).
static int __cdecl MpSpawn_DispatchThreeCategories(int context)
{
    void* first = nullptr;
    (void)first;

    // Category 1
    {
        int globalValue = GetGlobalDword_73C020();
        sub_73C050(reinterpret_cast<void*>(globalValue), nullptr, 1, context);
    }
    // Category 2
    {
        int globalValue = GetGlobalDword_73C020();
        sub_73C050(reinterpret_cast<void*>(globalValue), nullptr, 2, context);
    }
    // Category 3
    {
        int globalValue = GetGlobalDword_73C020();
        return reinterpret_cast<int>(sub_73C050(reinterpret_cast<void*>(globalValue), nullptr, 3, context));
    }
}

HOOK(0x005512D0, MpSpawn_DispatchThreeCategories, "auto");
