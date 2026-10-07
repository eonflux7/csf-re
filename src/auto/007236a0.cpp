#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// __thiscall (this = ecx, one stack arg), ret 4
CSF_FUNC(int*, __fastcall, Sub7234F0, 0x7234F0, (int* self, void* edx, int* a2));

// Forwards to sub_7234F0 only when the first argument is non-null; otherwise returns
// an undefined (null) pointer, matching the original leaving eax untouched.
static int* __stdcall ForwardTo7234F0(int* a1, int* a2)
{
    if (a1)
        return Sub7234F0(a1, nullptr, a2);
    return nullptr;
}

HOOK(0x007236A0, ForwardTo7234F0, "auto");
