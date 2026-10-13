#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, AiState_SetTaskState, 0x004DBFE0,
         (void* self, void* edx, int a2));

static int __fastcall AiState_ResetLinkedTaskState(void* self, void* /*edx*/, int state)
{
    void* linkedTask = CSF_AT(void*, self, 0x08);
    if (!linkedTask)
        return 0;

    void* currentState = CSF_AT(void*, linkedTask, 0x0C);
    int currentStateId = currentState ? CSF_AT(int, currentState, 0x10) : 0;

    if (state != currentStateId)
        AiState_SetTaskState(linkedTask, nullptr, state);

    return state;
}

HOOK(0x004DC380, AiState_ResetLinkedTaskState, "auto");
