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

// 0x004AA040 (6 bytes) — returns the literal string "TVigilar".
// Disassembly:
//   004aa040  mov eax, 0x83640c   ; address of the char literal
//   004aa045  ret                 ; __cdecl, pops no stack bytes
static const char* __cdecl GetTVigilarString()
{
    return "TVigilar";
}

HOOK(0x4AA040, GetTVigilarString, "auto");
