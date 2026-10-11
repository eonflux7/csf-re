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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(char, __fastcall, AiCoverAttackTasks_IsMovementComplete, 0x004B9950, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the linked object's value minus 4 (the sub-object used by the AI timer), or nullptr.
static void* GetLinkedObjectSubobject(int linkedOwner)
{
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(linkedOwner));
    int linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    if (linkedValue)
        return reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue - 4));
    return nullptr;
}

// Ticks the cover-attack movement timer (field at +0x69C) by the frame delta;
// when it runs out while movement is not complete, resets the linked task state.
static void __stdcall AiCore_TickCoverAttackMovementTimer(int a1, float a2)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    if (!AiCoverAttackTasks_IsMovementComplete(GetLinkedObjectSubobject(a1), nullptr))
    {
        void* timerOwner = GetLinkedObjectSubobject(a1);
        CSF_AT(float, timerOwner, 0x69C) = CSF_AT(float, timerOwner, 0x69C) - a2;

        void* checkOwner = GetLinkedObjectSubobject(a1);
        if (CSF_AT(float, checkOwner, 0x69C) < g_fZero)
        {
            int linkedField10 = GetLinkedObjectField10(self, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 12);
        }
    }
}

HOOK(0x4BCFB0, AiCore_TickCoverAttackMovementTimer, "auto");
