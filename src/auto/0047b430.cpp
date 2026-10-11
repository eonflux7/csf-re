#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Original function at 0x0047B430 (module ai_attack_tasks).
// Forwards a fixed command to the object at owner+0x14, then returns the result of a second command.
// No stack args; both callees are __thiscall via vtable.

static int __fastcall AiAttackTasks_IssueFireCommandAndReturn(void* self, void* /*edx*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* target = CSF_AT(void*, owner, 0x14);

    // target->vtbl[35](18, 0, -1, 0)   (byte offset 0x8C)
    ((void (__fastcall*)(void*, void*, int, int, int, int))
        reinterpret_cast<void*>(CSF_AT(void**, target, 0)[0x8C / 4]))(
        target, nullptr, 0x12, 0, -1, 0);

    // return target->vtbl[45](1)       (byte offset 0xB4)
    owner = CSF_AT(void*, self, 0x08);
    target = CSF_AT(void*, owner, 0x14);
    return ((int (__fastcall*)(void*, void*, int))
        CSF_AT(void**, target, 0)[0xB4 / 4])(target, nullptr, 1);
}

HOOK(0x0047B430, AiAttackTasks_IssueFireCommandAndReturn, "auto");
