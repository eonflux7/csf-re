#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char, __fastcall, AiAttackTasks_SetActionFlag, 0x0048C9D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* edx, int));
CSF_FUNC(void, __fastcall, ForwardToAttachedObject, 0x0048E5F0, (void* self, void* edx, void* owner));

// Clears bit 0 of the owner's flags word (owner = self->+0x08) and returns the new value.
static uint32_t __fastcall AiAutonomous_ClearActionFlagAndReturnFlags(void* self, void* /*edx*/)
{
    // AiAttackTasks_SetActionFlag(-1) on the task at owner->+0x14->+0x204
    AiAttackTasks_SetActionFlag(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204),
        nullptr,
        static_cast<char>(-1));

    // C_DecrementCounterAt30(task, self)
    C_DecrementCounterAt30(
        CSF_AT(void*, CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14), 0x204),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    // ForwardToAttachedObject(self) with ecx = owner->+0x14
    ForwardToAttachedObject(
        CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14),
        nullptr,
        self);

    // owner->flags &= ~1; return owner->flags
    void* owner = CSF_AT(void*, self, 0x08);
    uint32_t flags = CSF_AT(uint32_t, owner, 0x18) & 0xFFFFFFFEu;
    CSF_AT(uint32_t, owner, 0x18) = flags;
    return flags;
}

HOOK(0x004D7FF0, AiAutonomous_ClearActionFlagAndReturnFlags, "auto");
