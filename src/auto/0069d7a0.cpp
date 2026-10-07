#include <cstdint>
#include "hook.h"

// Sets the dword pointed to by a1 to 0xFFFFFFFF (invalid/-1) and returns a1.
// Straightforward __cdecl helper: no `ret N`, single stack argument.
static uint32_t* __cdecl InitializeToInvalidAndReturn(uint32_t* a1)
{
    *a1 = 0xFFFFFFFFu;
    return a1;
}

HOOK(0x0069D7A0, InitializeToInvalidAndReturn, "auto");
