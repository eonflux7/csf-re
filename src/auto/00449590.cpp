#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, ForwardFourArguments, 0x00449130,
         (int a1, int a2, uint32_t* a3, int a4));

static uint32_t* __cdecl ForwardThreeArguments(int a1, int a2, uint32_t* a3)
{
    // The original forwards an unspecified stack value as the fourth argument.
    return ForwardFourArguments(a1, a2, a3, 0);
}

HOOK(0x00449590, ForwardThreeArguments, "auto");
