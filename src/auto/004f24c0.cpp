#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4. The source element pointer is
// passed through untouched, so the stdcall int argument is cast back to a pointer.
CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_CopyConstruct_4F2410, 0x004F2410, (C_0082d410* self, void* /*edx*/, C_0082d410* source));

// Copy-constructs the element a2 into a1 when a1 is non-null; otherwise nothing
// is done. The original returned an uninitialized eax in that case; this returns
// nullptr instead.
static C_0082d410* __stdcall HudStringShow_CopyElement(C_0082d410* a1, int a2)
{
    if (a1)
        return C_0082d410_CopyConstruct_4F2410(a1, nullptr, reinterpret_cast<C_0082d410*>(static_cast<intptr_t>(a2)));
    return nullptr;
}

HOOK(0x004F24C0, HudStringShow_CopyElement, "auto");
