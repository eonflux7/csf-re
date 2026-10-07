#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the size in bytes of a C_00862e88 instance (0x68 = 104).
static int __cdecl C_00862e88_GetSize()
{
    return 104;
}

HOOK(0x6FC170, C_00862e88_GetSize, "auto");
