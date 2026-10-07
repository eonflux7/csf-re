// Reimplementation of sub_6B3D60 (0x006B3D60) - size 6 bytes
// Commandos: Strike Force (Pyro Studios) - hook DLL
//
// The original simply materializes the constant 0x1C (28) into EAX and returns.
// This matches sizeof(C_0086287c) == 0x1C, so it is almost certainly a
// "get object size" helper / vtable slot for the C_0086287c hierarchy.
// It does not touch `this`, so it is modeled as a plain __cdecl free function.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the size in bytes of a C_0086287c instance.
static int __cdecl C_0086287c_GetObjectSize()
{
    return 28;
}

HOOK(0x6B3D60, C_0086287c_GetObjectSize, "auto");
