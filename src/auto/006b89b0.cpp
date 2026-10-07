#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

// Global float slot written by the original function.
CSF_GLOBAL(float, g_Float_8A7E40, 0x8A7E40);

// Stores the incoming float into the global at 0x8A7E40 and returns its raw
// 32-bit representation in eax (the original moves the argument's dword into
// eax and leaves it there).
static int __cdecl SetGlobalFloat_8A7E40(float value)
{
    g_Float_8A7E40 = value;

    int raw_bits;
    std::memcpy(&raw_bits, &value, sizeof(raw_bits));
    return raw_bits;
}

HOOK(0x006B89B0, SetGlobalFloat_8A7E40, "auto");
