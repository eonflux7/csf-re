#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg a2), ret 4
CSF_FUNC(int*, __fastcall, Sub723B00, 0x00723B00, (int* self, void* edx, int a2));

// Forwards to sub_723B00 (with a1 as this) only when the first argument is non-null.
// When a1 is null the original jumps straight to the epilogue, leaving eax untouched
// (undefined); we mirror the verified clone by returning nullptr.
static int* __stdcall CallSub723B00IfNonNull(int* a1, int a2)
{
    if (a1)
        return Sub723B00(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x00724020, CallSub723B00IfNonNull, "auto");
