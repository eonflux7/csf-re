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
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Clears the linked cover task's flag byte (value-4 + 0x6CC) and resets its linked task state.
// Original: int __stdcall sub_4AC230(int a1, int a2, int a3); ret 12
static int __stdcall AiCoverTasks_ResetLinkedCoverTask(uint32_t a1, uint32_t a2, uint32_t a3)
{
    (void)a2;
    (void)a3;

    void* owner = reinterpret_cast<void*>(a1);

    int linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    int flagBase = linkedValue ? linkedValue - 4 : 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(flagBase), 0x6CC) = 0;

    int linkedField10 = GetLinkedObjectField10(owner, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(owner, nullptr, linkedField10 + 8);
}

HOOK(0x004AC230, AiCoverTasks_ResetLinkedCoverTask, "auto");
