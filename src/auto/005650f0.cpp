#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(uint32_t, __fastcall, NetProfile_SetSlotIfInRange, 0x00518490, (void* self, void* , uint32_t value));
CSF_FUNC(unsigned int, __cdecl, StringFormat_CopyTrucoStringToWide, 0x00695590, (char* src, uint16_t* dst, int maxCount));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetIndexedTableValue_714C20, 0x00714C20, (int* self, void* , int rowIndex, int columnIndex));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(std::uint32_t*, __fastcall, NetSession_SetTextInputOrDefault, 0x0073C320, (void* self, void* , std::int16_t* text));

CSF_GLOBAL(const char*, g_pszMenu_P_LbCuLa, 0x00899B34);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuCr, 0x00899B38);
CSF_GLOBAL(const char*, g_pszMenu_P_LbCuMo, 0x00899B3C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbMePr, 0x00899B40);
CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);
CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x0089A5EC);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(int, g_MpAccountMode, 0x0089A5F4);

static int __fastcall CMenuAccounts_HandleStateOne(
    CMenuAccounts* self, void* /*edx*/, int /*stateArgument*/)
{
    alignas(8) uint8_t accountNameBytes[0x48];

    SceneLoader_NoOp();
    if (self->field_0048)
    {
        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(loader, nullptr, 1279341108, 1, 1, 0);
        self->field_0048 = 0;
    }

    int globalValue = GetGlobalValue_8B056C();
    void* profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
    int result = ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, profiles, 0)[0x2C / 4])(profiles, nullptr);

    if (result || self->field_0030)
    {
        auto target = self->vtbl[0x60 / 4];
        self->field_0030 = 0;
        result = ((int(__fastcall*)(CMenuAccounts*, void*))target)(self, nullptr);
    }

    if (self->field_002c)
    {
        GetGlobalValue_8B056C();
        globalValue = GetGlobalValue_8B056C();
        profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
        if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, profiles, 0)[0x24 / 4])(profiles, nullptr))
        {
            void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
            result = GameBddLoader_DispatchMenuRequest(
                loader, nullptr, 1229991986, 1, 1, 0);
        }
        else
        {
            g_SelectedAccountIndex = -1;
            g_MpAccountMode = 0;

            void* screen = reinterpret_cast<void*>(self->field_0038);
            ((void(__fastcall*)(void*, void*, CMenuAccounts*, int))
                CSF_AT(void**, screen, 0)[0x60 / 4])(screen, nullptr, self, 0);

            const char* screenName = g_pszMenu_P_LbCuLa;
            if (g_MpNetworkType != 2)
                screenName = g_pszMenu_P_LbCuCr;

            screen = reinterpret_cast<void*>(self->field_0038);
            result = ((int(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, screen, 0)[0x58 / 4])(screen, nullptr, screenName);
        }
        self->field_002c = 0;
    }

    if (self->field_002d)
    {
        result = CGuiListBox_GetItemCount_714860(
            reinterpret_cast<CGuiListBox*>(self->field_0014), nullptr);
        if (result > 0)
        {
            GetGlobalValue_8B056C();
            globalValue = GetGlobalValue_8B056C();
            profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
            void** profileVtable = CSF_AT(void**, profiles, 0);

            int row = GetStoredValueAtIndex(
                reinterpret_cast<void*>(self->field_0014), nullptr, 0);
            int accountIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0014), nullptr, row, 0);
            void* account = ((void*(__fastcall*)(void*, void*, int))
                profileVtable[0x30 / 4])(profiles, nullptr, accountIndex);
            result = reinterpret_cast<int>(account);

            if (result)
            {
                row = GetStoredValueAtIndex(
                    reinterpret_cast<void*>(self->field_0014), nullptr, 0);
                g_SelectedAccountIndex = GetIndexedTableValue_714C20(
                    reinterpret_cast<int*>(self->field_0014), nullptr, row, 0);
                g_MpAccountMode = 1;

                void* screen = reinterpret_cast<void*>(self->field_0038);
                ((void(__fastcall*)(void*, void*, CMenuAccounts*, int))
                    CSF_AT(void**, screen, 0)[0x60 / 4])(screen, nullptr, self, 0);

                const char* screenName = g_pszMenu_P_LbCuLa;
                if (g_MpNetworkType != 2)
                    screenName = g_pszMenu_P_LbCuMo;

                screen = reinterpret_cast<void*>(self->field_0038);
                result = ((int(__fastcall*)(void*, void*, const char*))
                    CSF_AT(void**, screen, 0)[0x58 / 4])(screen, nullptr, screenName);
                self->field_002c = 0;
            }
        }
        self->field_002d = 0;
    }

    if (self->field_002e)
    {
        result = CGuiListBox_GetItemCount_714860(
            reinterpret_cast<CGuiListBox*>(self->field_0014), nullptr);
        if (result > 0)
        {
            result = ((int(__fastcall*)(CMenuAccounts*, void*))
                self->vtbl[0x68 / 4])(self, nullptr);
        }
        self->field_002e = 0;
    }

    if (self->field_002f)
    {
        if (CGuiListBox_GetItemCount_714860(
            reinterpret_cast<CGuiListBox*>(self->field_0014), nullptr) > 0)
        {
            GetGlobalValue_8B056C();
            globalValue = GetGlobalValue_8B056C();
            profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
            void** profileVtable = CSF_AT(void**, profiles, 0);

            int row = GetStoredValueAtIndex(
                reinterpret_cast<void*>(self->field_0014), nullptr, 0);
            int accountIndex = GetIndexedTableValue_714C20(
                reinterpret_cast<int*>(self->field_0014), nullptr, row, 0);
            if (((void*(__fastcall*)(void*, void*, int))
                profileVtable[0x30 / 4])(profiles, nullptr, accountIndex))
            {
                row = GetStoredValueAtIndex(
                    reinterpret_cast<void*>(self->field_0014), nullptr, 0);
                g_SelectedAccountIndex = GetIndexedTableValue_714C20(
                    reinterpret_cast<int*>(self->field_0014), nullptr, row, 0);
            }
        }
        result = ((int(__fastcall*)(CMenuAccounts*, void*))
            self->vtbl[0x54 / 4])(self, nullptr);
        self->field_002f = 0;
    }

    if (g_dwAccountEditScreenState == 1 && g_SelectedAccountIndex != -1)
    {
        globalValue = GetGlobalValue_8B056C();
        profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
        NetProfile_SetSlotIfInRange(profiles, nullptr, g_SelectedAccountIndex);

        globalValue = GetGlobalValue_8B056C();
        profiles = CSF_AT(void*, reinterpret_cast<void*>(globalValue), 0x14);
        uint8_t* account = ((uint8_t*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, profiles, 0)[0x30 / 4])(
                profiles, nullptr, g_SelectedAccountIndex);

        if (account)
        {
            g_MpNetworkType = 3 - (CSF_AT(uint8_t, account, 0) != 0);

            void* screen = reinterpret_cast<void*>(self->field_0038);
            ((void(__fastcall*)(void*, void*, CMenuAccounts*, int))
                CSF_AT(void**, screen, 0)[0x60 / 4])(screen, nullptr, self, 0);

            StringFormat_CopyTrucoStringToWide(
                reinterpret_cast<char*>(account + 8),
                reinterpret_cast<uint16_t*>(accountNameBytes), 33);
            void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
            NetSession_SetTextInputOrDefault(
                session, nullptr, reinterpret_cast<std::int16_t*>(accountNameBytes));

            screen = reinterpret_cast<void*>(self->field_0038);
            result = ((int(__fastcall*)(void*, void*, const char*))
                CSF_AT(void**, screen, 0)[0x58 / 4])(
                    screen, nullptr, g_pszMenu_P_LbMePr);
            g_dwAccountEditScreenState = 3;
        }
        else
        {
            void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
            return GameBddLoader_DispatchMenuRequest(
                loader, nullptr, 1279341109, 1, 1, 0);
        }
    }

    return result;
}

HOOK(0x005650F0, CMenuAccounts_HandleStateOne, "auto");
