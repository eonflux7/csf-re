#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Attack-task dispatch: fires the first sub-object's method, then tail-jumps
// into the second sub-object's slot 0x70 with the caller's arguments forwarded.

static hook::Tail __fastcall ForwardFireTask(void* self, const uint32_t* /*args*/)
{
    // eax = this->field_0008 (owner object A)
    void* ownerA = CSF_AT(void*, self, 0x08);

    // ecx = A->field_0014 -> B; ecx = B->field_0048 -> C
    void* subB = CSF_AT(void*, ownerA, 0x14);
    void* subC = CSF_AT(void*, subB, 0x48);

    // C->vtbl[0x38 / 4](C, A)  (__thiscall, one argument: A)
    void** vtableC = CSF_AT(void**, subC, 0);
    using FireFn = void(__fastcall*)(void*, void*, void*);
    reinterpret_cast<FireFn>(vtableC[0x38 / 4])(subC, nullptr, ownerA);

    // eax = this->field_0008 again (re-read after the call)
    ownerA = CSF_AT(void*, self, 0x08);

    // ecx = A->field_0014 -> B; ecx = B->field_0044 -> D
    subB = CSF_AT(void*, ownerA, 0x14);
    void* subD = CSF_AT(void*, subB, 0x44);

    // jmp D->vtbl[0x70 / 4] with ecx = D, caller's stack arguments forwarded
    return hook::tail_jump(subD, (*reinterpret_cast<void***>(subD))[0x70 / 4]);
}

HOOK_TAIL(0x47B2C0, ForwardFireTask, "ai_attack_tasks");
