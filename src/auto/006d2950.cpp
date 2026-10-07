#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x006D2950: trivial accessor returning the class size constant (228).
static int __cdecl GetClassSize()
{
    return 228;
}

HOOK(0x6D2950, GetClassSize, "auto");
