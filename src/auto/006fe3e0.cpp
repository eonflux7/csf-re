#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global object whose address is returned by the target function.
CSF_GLOBAL(uint32_t, g_Value8BB420, 0x008BB420);

// 0x006FE3E0: returns the address of g_Value8BB420, ignoring its single argument.
static uint32_t* __stdcall GetValue8BB420Address(int /*unused*/)
{
    return &g_Value8BB420;
}

HOOK(0x6FE3E0, GetValue8BB420Address, "auto");
