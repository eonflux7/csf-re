#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Float constant (0.7f) stored in the game's read-only data at 0x0082E068.
CSF_GLOBAL(float, g_Const_0082E068, 0x0082E068);

// Loads the constant float 0x0082E068 and returns it (as double, via the x87
// register, exactly like the original `fld dword ptr [0x82e068]`).
// The `this` pointer is never touched by the original code.
static double __fastcall C_008300c0_GetConstantFloat(C_008300c0* /*self*/, void* /*edx*/)
{
    return g_Const_0082E068;
}

HOOK(0x00429190, C_008300c0_GetConstantFloat, "auto");
