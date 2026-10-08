#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0, 0x005231D0, (int globalValue, void* edx, int requestValue));
CSF_FUNC(void*, __fastcall, CMenuProfiles_CheckSchedulerProxyAndDisable, 0x0053BAA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

static int QuerySchedulerProxyCode()
{
    void* globalState = GetGlobalState_50D190();
    return SchedulerBase_GetSchedulerProxy(globalState, nullptr);
}

static void CMenuProfiles_ResetStateAndNotifyTarget(void* self)
{
    void* targetObject = CSF_AT(void*, self, 0x24);
    CSF_AT(int, self, 0x38) = -1;
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, targetObject, 0)[42])(targetObject, nullptr, 0);
}

static void __fastcall CMenuProfiles_SetPendingValue(void* self, void* /*edx*/, int requestValue)
{
    CSF_AT(int, self, 0x34) = requestValue;

    const int stateAtEntry = CSF_AT(int, self, 0x38);
    if (stateAtEntry == 1)
    {
        const int proxyCode = QuerySchedulerProxyCode();
        if (proxyCode == 1)
            CSF_AT(int, self, 0x38) = 0;
        else if (proxyCode == 2)
            CMenuProfiles_ResetStateAndNotifyTarget(self);
    }
    else if (stateAtEntry == 3)
    {
        const int proxyCode = QuerySchedulerProxyCode();
        if (proxyCode == 1)
            CSF_AT(int, self, 0x38) = 4;
        else if (proxyCode == 2)
            CMenuProfiles_ResetStateAndNotifyTarget(self);
    }
    else if (stateAtEntry == 6)
    {
        CMenuProfiles_CheckSchedulerProxyAndDisable(self, nullptr);
    }

    // The original pushes the stored value before calling GetGlobalValue_521BB0 and passes it as the stack argument.
    const int storedValue = CSF_AT(int, self, 0x34);
    const int globalValue = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(globalValue, nullptr, storedValue);
}

HOOK(0x0053BAE0, CMenuProfiles_SetPendingValue, "auto");
