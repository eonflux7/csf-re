#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4
CSF_FUNC(int*, __fastcall, MenuExtras_CopyConstructEntry, 0x00546700, (int* self, void* edx, int* a2));

// Forwards both arguments to sub_546700 (with a1 as the this pointer) when a1 is
// non-null; otherwise nothing is done and eax is left as the undefined value the
// original returned.
static int* __stdcall MenuExtras_ForwardIfNonNull(int* a1, int* a2)
{
    if (a1)
        return MenuExtras_CopyConstructEntry(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x00546DD0, MenuExtras_ForwardIfNonNull, "auto");
