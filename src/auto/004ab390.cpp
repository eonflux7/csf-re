#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address: the original's tail-jump target, with matching __stdcall signature.
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

// Plain forwarder (never reads ecx). The original is `push ebp / mov ebp,esp / pop ebp / jmp 0x4DBD50`,
// so the callee's arguments and its `ret 8` pass straight through unchanged.
static int __stdcall MovilAPunto_ForwardToCoverAttackTasksUpdate(int a1, int a2)
{
    return AiCoverAttackTasks_UpdateCharacterTasks(a1, a2);
}

HOOK(0x004AB390, MovilAPunto_ForwardToCoverAttackTasksUpdate, "auto");
