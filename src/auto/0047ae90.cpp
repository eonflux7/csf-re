#include <cstdint>
#include "globals.h"
#include "hook.h"

// TDisparar / TAtacarRafaga / TAtacar / TFunctionRecargar / TApuntar (ai_attack_tasks).
// Stores the two stack arguments into the first two dwords of the object
// (thiscall: ecx = this) and returns the first argument.
static int __fastcall AiAttackTasks_SetFirstTwoFieldsReturnFirst(void* self, void* /*edx*/, int firstValue, int secondValue) {
    CSF_AT(int, self, 0x0) = firstValue;
    CSF_AT(int, self, 0x4) = secondValue;
    return firstValue;
}

HOOK(0x0047AE90, AiAttackTasks_SetFirstTwoFieldsReturnFirst, "auto");
