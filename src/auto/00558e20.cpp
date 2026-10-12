#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, sub_558B00, 0x00558B00, (void* self, void* edx, int a2));

// Copies the first dword of the source block into the destination's first dword,
// then forwards the destination's sub-block (at +4) and the source's second dword
// address (source + 4) to sub_558B00. Returns whatever sub_558B00 returns, or the
// null destination pointer when the destination is null.
static int* __stdcall MpRules_StoreValueAndForwardToInner(int a1, int* a2)
{
    int* result = reinterpret_cast<int*>(static_cast<uintptr_t>(a1));
    if (a1 != 0)
    {
        *reinterpret_cast<int*>(static_cast<uintptr_t>(a1)) = *a2;
        return sub_558B00(
            reinterpret_cast<void*>(static_cast<uintptr_t>(a1) + 4),
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(a2 + 1)));
    }
    return result;
}

HOOK(0x00558E20, MpRules_StoreValueAndForwardToInner, "auto");
