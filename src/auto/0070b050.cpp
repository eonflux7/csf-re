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

// Callee at 0x6FF9F0: __thiscall with one float stack argument (ret 4).
CSF_FUNC(char, __fastcall, WaterBase_HandleFloatParam, 0x006FF9F0, (void* self, void* edx, float a2));

// 0x70B050: thin __thiscall forwarder. The original is `push ebp / mov ebp,esp / pop ebp / jmp 0x6FF9F0`,
// so it tail-jumps and the callee's `ret 4` pops the float argument.
static char __fastcall C_00863678_ForwardFloatToBase(C_00863678* self, void* /*edx*/, float a2)
{
    return WaterBase_HandleFloatParam(self, nullptr, a2);
}

HOOK(0x0070B050, C_00863678_ForwardFloatToBase, "auto");
