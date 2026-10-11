#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CoverTask_IsLinkedObjectReady, 0x004AF3D0, (void* self, void* edx));
// Real calling convention is __thiscall (ecx = this) with 3 stack args (ret 12).
CSF_FUNC(void, __fastcall, CoverTask_NotifyAttack, 0x004C72A0, (void* self, void* edx, int provider, char flagB, char flagA));
// Real calling convention is __thiscall (ecx = this) with 1 stack arg (ret 4).
CSF_FUNC(int, __fastcall, CoverTask_StartMove, 0x004C83B0, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverTask_SetTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverTask_Link, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void __fastcall CoverAttack_UpdateState(void* self, void* /*edx*/, C_0085e760* task, int /*a2*/, int /*a3*/)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* linkedObj = linkedValue ? (void*)(intptr_t)(linkedValue - 4) : nullptr;

    if (CoverTask_IsLinkedObjectReady(linkedObj, nullptr))
    {
        int field10 = GetLinkedObjectField10(task, nullptr);
        CoverTask_Link(task, nullptr, field10 + 4);

        int entry = ResolveEntryLookupValue(task, nullptr);
        void* provider = GetCachedObjectProvider((void*)(intptr_t)entry, nullptr);

        int target = GetLinkedObjectField10(task, nullptr) + 4;

        int state1 = GetGlobalState_50D190();
        char flagA = GetStateValue((void*)(intptr_t)state1, nullptr, 10) > 6;
        int state2 = GetGlobalState_50D190();
        char flagB = GetStateValue((void*)(intptr_t)state2, nullptr, 10) > 6;

        CoverTask_NotifyAttack((void*)(intptr_t)target, nullptr, (int)(intptr_t)provider, flagB, flagA);
    }
    else
    {
        int state = GetGlobalState_50D190();
        if ((unsigned int)GetStateValue((void*)(intptr_t)state, nullptr, 100) >= 0x1Eu)
            CoverTask_SetTarget(task, nullptr, 0);
        else
            CoverTask_StartMove(self, nullptr, (int)(intptr_t)task);
    }
}

HOOK(0x004CA480, CoverAttack_UpdateState, "auto");
