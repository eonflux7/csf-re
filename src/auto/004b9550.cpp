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

// 0x004B9550 (6 bytes) - returns the constant name string "TFncMacroOrden".
// Disassembly:
//   mov eax, 0x83A9CC
//   ret
// No stack arguments popped (ret 0), so this is a __thiscall-style member
// function taking only `this` (ecx); edx unused.
static const char* __fastcall TFncMacroOrden_GetFncMacroOrdenName(TFncMacroOrden* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x0083A9CC, "TFncMacroOrden");
}

HOOK(0x004B9550, TFncMacroOrden_GetFncMacroOrdenName, "auto");
