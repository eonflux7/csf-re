#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(unsigned int, __fastcall, MenuMpAccounts_OnStateTransition, 0x0055C2B0, (void* self, void* , int));
CSF_FUNC(int, __fastcall, MenuAccounts_ProcessSchedulerProxyState, 0x0055C320, (void* self, void* , int));
CSF_FUNC(int, __fastcall, CMenuAccountEdit_LoadSelectedAccount, 0x0055C830, (CMenuAccountEdit* self, void* , int));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_ProcessAccountEditState6, 0x0055CB10, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, MenuAccounts_DispatchProfileState, 0x0055CBD0, (void* self, void* , int));
CSF_FUNC(char, __fastcall, MenuMpAccounts_SubmitAccountEdit, 0x0055CCC0, (void* self, void* , int));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_ProcessAccountEditState7, 0x0055D090, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MenuAccounts_ResetOnSelect, 0x0055D130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, C_0085839c_UpdateNetworkManager, 0x0073D550, (C_0085839c* self, void* , int));

static void __fastcall CMenuAccountEdit_Update(
    CMenuAccountEdit* self, void* /*edx*/, int deltaBits)
{
    if (g_MpNetworkType == 3)
    {
        auto* networkManager =
            reinterpret_cast<C_0085839c*>(GetGlobalDword_73C020());
        C_0085839c_UpdateNetworkManager(networkManager, nullptr, deltaBits);

        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, networkState, 0)[0x18 / 4])(
                networkState, nullptr, deltaBits);
    }

    auto* timeState = reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    if (self->field_0031)
    {
        auto* parent = reinterpret_cast<CGuiSlider*>(self->field_0018);
        if (parent)
            CGuiSlider_ForwardSelfToParent(parent, nullptr);
        self->field_0031 = 0;
    }

    switch (self->field_0280)
    {
    case 0u:
        CMenuAccountEdit_LoadSelectedAccount(self, nullptr, deltaBits);
        break;

    case 1u:
        if (self->field_0030)
        {
            MenuAccounts_ResetOnSelect(self, nullptr);
            self->field_0030 = 0;
        }
        break;

    case 2u:
        MenuAccounts_ProcessSchedulerProxyState(self, nullptr, deltaBits);
        break;

    case 3u:
    {
        auto* globalState =
            reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
        if (C_00861120_GetSchedulerProxy(globalState, nullptr))
            self->field_0280 = 1;
        break;
    }

    case 4u:
    {
        auto* globalState =
            reinterpret_cast<C_00861120*>(GetGlobalState_50D190());
        if (C_00861120_GetSchedulerProxy(globalState, nullptr))
            self->vApplyChangesAndReload();
        break;
    }

    case 5u:
        MenuMpAccounts_SubmitAccountEdit(self, nullptr, deltaBits);
        break;

    case 6u:
        MenuMpAccounts_ProcessAccountEditState6(self, nullptr, deltaBits);
        break;

    case 7u:
        MenuMpAccounts_ProcessAccountEditState7(self, nullptr, deltaBits);
        break;

    case 8u:
        MenuMpAccounts_OnStateTransition(self, nullptr, deltaBits);
        break;

    case 9u:
        MenuAccounts_DispatchProfileState(self, nullptr, deltaBits);
        break;

    default:
        return;
    }
}

HOOK(0x0055D2E0, CMenuAccountEdit_Update, "auto");
