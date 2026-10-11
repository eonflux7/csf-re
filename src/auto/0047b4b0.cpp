#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original function at 0x0047B4B0 (module ai_attack_tasks).
// Sends command 0x11 with args (0, -1, 0) to the object at owner+0x14, then
// returns the result of a second query command (arg 0) on the same object.
// No stack args; both callees are __thiscall via vtable.

static int __fastcall AiAttackTasks_SendCommandAndQueryStatus(void* self, void* /*edx*/) {
    void* owner = CSF_AT(void*, self, 0x08);
    void* target = CSF_AT(void*, owner, 0x14);

    // target->vtbl[35](17, 0, -1, 0)   (byte offset 0x8C)
    ((void (__fastcall*)(void*, void*, int, int, int, int))
        CSF_AT(void**, target, 0)[0x8C / 4])(
        target, nullptr, 0x11, 0, -1, 0);

    // return target->vtbl[45](0)       (byte offset 0xB4)
    owner = CSF_AT(void*, self, 0x08);
    target = CSF_AT(void*, owner, 0x14);
    return ((int (__fastcall*)(void*, void*, int))
        CSF_AT(void**, target, 0)[0xB4 / 4])(target, nullptr, 0);
}

HOOK(0x0047B4B0, AiAttackTasks_SendCommandAndQueryStatus, "auto");
