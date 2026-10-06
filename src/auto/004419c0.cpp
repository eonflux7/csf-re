#include <cstdint>
#include "globals.h"
#include "hook.h"

// Function at 0x004419C0 is a trivial accessor that returns the 32-bit
// global value stored at 0x008AFC80. It takes no arguments and is __cdecl
// (the sole `ret` has no immediate, so the caller cleans up).
CSF_GLOBAL(int, g_Value_8afc80, 0x8AFC80);

static int __cdecl GetValue_8afc80()
{
    return g_Value_8afc80;
}

HOOK(0x4419C0, GetValue_8afc80, "auto");
