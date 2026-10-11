#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4.
CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_CopyConstruct_500FE0, 0x00500FE0, (C_0082d410* self, void* /*edx*/, C_0082d410** a2));

// Copy-constructs from a2 into the object at a1 when a1 is non-null; otherwise nothing is done.
// The original returned an uninitialized eax in the null case; this returns 0 instead.
static int __stdcall C_0082d410_CopyConstructIfNonNull(int a1, int a2)
{
    if (a1)
    {
        C_0082d410* self = reinterpret_cast<C_0082d410*>(static_cast<intptr_t>(a1));
        C_0082d410** source = reinterpret_cast<C_0082d410**>(static_cast<intptr_t>(a2));
        C_0082d410* result = C_0082d410_CopyConstruct_500FE0(self, nullptr, source);
        return static_cast<int>(reinterpret_cast<intptr_t>(result));
    }
    return 0;
}

HOOK(0x00755CC0, C_0082d410_CopyConstructIfNonNull, "auto");
