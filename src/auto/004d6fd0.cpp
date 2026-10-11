#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(char, __fastcall, AiAttackTasks_SetActionFlag, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* edx, int a2));

// Returns the attack-task object held by a TCharTask: this->owner(+0x08)->sub(+0x14)->object(+0x204).
static void* TCharTask_GetAttackTaskObject(TCharTask* self) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* sub = CSF_AT(void*, owner, 0x14);
    return CSF_AT(void*, sub, 0x204);
}

static int __fastcall TCharTask_PrepareAndDecrementAttackCounter(TCharTask* self, void* /*edx*/) {
    CharTaskBase_Prepare(self, nullptr);
    AiAttackTasks_SetActionFlag(TCharTask_GetAttackTaskObject(self), nullptr, (char)-1);
    return C_DecrementCounterAt30(TCharTask_GetAttackTaskObject(self), nullptr, (int)self);
}

HOOK(0x4D6FD0, TCharTask_PrepareAndDecrementAttackCounter, "auto");
