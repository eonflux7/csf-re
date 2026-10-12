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

// CoverAttack_SetState_4DC150: __thiscall with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

// Original: loads the first stack argument into ecx (this), pushes 0 and
// calls CoverAttack_SetState_4DC150. The other two arguments are ignored.
static int __stdcall AiMoveTasks_SetCoverAttackStateDefault(int a1, int /*a2*/, int /*a3*/)
{
    return CoverAttack_SetState_4DC150(reinterpret_cast<void*>(a1), nullptr, 0);
}

HOOK(0x004AB460, AiMoveTasks_SetCoverAttackStateDefault, "auto");
