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

// Original: sub_4AD290 (__stdcall, 1 arg, ret 4)
static int __stdcall AiOrder_ClearLinkedOrderState(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int subObjectBase = linkedValue ? linkedValue - 4 : 0;

    void* subObject = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(subObjectBase)), 0x2C);

    // vtable slot 0xB4 / 4 = 45, thiscall with one argument (1)
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[45])(subObject, nullptr, 1);

    int linkedField10 = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 8);
}

HOOK(0x004AD290, AiOrder_ClearLinkedOrderState, "auto");
