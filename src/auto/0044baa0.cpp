#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address; the callee pops its 4-byte stack argument (ret 4).
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_AppendEntityHandle, 0x0044B9A0, (void* self, void* edx, int a3));

// Original: push ebp; mov ebp,esp; add ecx,4; pop ebp; jmp 0x44B9A0
// Forwards to AppendEntityHandle on the sub-object at this+4, passing a2 through.
static uint32_t* __fastcall AiAttackTasks_AppendEntityHandleAtOffset4(void* self, void* /*edx*/, int a2)
{
    void* subObject = reinterpret_cast<uint8_t*>(self) + 4;
    return AiAttackTasks_AppendEntityHandle(subObject, nullptr, a2);
}

HOOK(0x0044BAA0, AiAttackTasks_AppendEntityHandleAtOffset4, "auto");
