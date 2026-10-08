#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (int* this = ecx, one stack arg), ret 4
CSF_FUNC(int*, __fastcall, ConstructPointerVectorCopy_715C30, 0x715C30, (int* self, void* edx, int a2));

// Forwards to sub_715C30 only when the first argument is non-null; otherwise returns
// an undefined (null) pointer just like the original left eax untouched.
static int* __stdcall CallHandlerIfNonNull(int* a1, int a2)
{
    if (a1)
        return ConstructPointerVectorCopy_715C30(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x00716060, CallHandlerIfNonNull, "auto");
