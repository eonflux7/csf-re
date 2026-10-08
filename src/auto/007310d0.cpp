#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (ecx = this, one stack arg, ret 4)
CSF_FUNC(int, __fastcall, ConstructPointerVectorCopy_730E00, 0x730E00, (int self, void* edx, int a2));

// Forwards (this, a2) to sub_730E00 only when the first argument is non-null;
// otherwise the original leaves eax untouched, which we mirror by returning 0.
static int __stdcall ForwardToHandlerIfNonNull(int a1, int a2)
{
    if (a1)
        return ConstructPointerVectorCopy_730E00(a1, nullptr, a2);
    return 0;
}

HOOK(0x007310D0, ForwardToHandlerIfNonNull, "auto");

