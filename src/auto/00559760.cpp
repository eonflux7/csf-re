#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_559400, 0x00559400, (void* self, void* edx, int a2));

// Copies the first dword of the source block into the destination's first dword,
// then forwards the destination's sub-block (at +4) and the address of the source's
// second dword (source + 4) to sub_559400. Returns whatever sub_559400 returns, or the
// null destination pointer when the destination is null.
static int __stdcall MpRules_StoreFirstDwordAndForwardToInner(int a1, int* a2)
{
    int result = a1;
    if (a1 != 0)
    {
        *reinterpret_cast<int*>(static_cast<uintptr_t>(a1)) = *a2;
        return sub_559400(
            reinterpret_cast<void*>(static_cast<uintptr_t>(a1) + 4),
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(a2 + 1)));
    }
    return result;
}

HOOK(0x00559760, MpRules_StoreFirstDwordAndForwardToInner, "auto");
