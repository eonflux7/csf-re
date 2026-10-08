#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ApplyHeldPositionDelta, 0x0055C1B0,
         (CMenuAccountEdit* self, void* edx));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, MenuMpAccounts_PrepareNetworkAccount, 0x006A7350, ());
CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, MenuMpAccounts_SetAccountScreen, 0x0073F120,
         (void* self, void* edx, void* screen));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(int, g_MpAccountMode, 0x0089A5F4);

static std::uint32_t MenuMpAccounts_FindControl(void* screen, const char* name)
{
    return reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xFC / 4])(screen, nullptr, name);
}

static int __fastcall CMenuAccountEdit_InitializeControls(
    CMenuAccountEdit* self, void* /*edx*/, int context, int /*unused*/)
{
    if (!self->field_0284)
    {
        void* manager = GetGlobalState_50D190();
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuAccountEdit*)>(
            CSF_AT(void**, manager, 0)[0x18 / 4])(manager, nullptr, self);
    }

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x50 / 4])(self, nullptr);

    self->field_0014 = static_cast<std::uint32_t>(context);
    MenuProgressInput_UpdateGlobalState(GetGlobalValue_521BB0(), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0284);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0284);
    std::uint32_t control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x008454E4, "BotonSalvarCambios"));
    screen = reinterpret_cast<void*>(self->field_0284);
    self->field_0018 = control;

    control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x00843B00, "BotonBack"));
    screen = reinterpret_cast<void*>(self->field_0284);
    self->field_001c = control;

    control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x008454D4, "EdNombreCuenta"));
    self->field_0020 = control;
    if (control)
    {
        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<void*>(control), nullptr);
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0020), 0x14A) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0020), 0x1C6) = 0;
    }

    screen = reinterpret_cast<void*>(self->field_0284);
    control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x008454C0, "EdPasswordCuenta"));
    self->field_0024 = control;
    if (control)
    {
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(control), 0x149) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0024), 0x14A) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0024), 0x1C6) = 0;
    }

    screen = reinterpret_cast<void*>(self->field_0284);
    control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x008454A8, "EdRetypePasswordCuenta"));
    self->field_0028 = control;
    if (control)
    {
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(control), 0x149) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0028), 0x14A) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0028), 0x1C6) = 0;
    }

    screen = reinterpret_cast<void*>(self->field_0284);
    control = MenuMpAccounts_FindControl(
        screen, CSF_STR(0x00845494, "EdOldPasswordCuenta"));
    self->field_002c = control;
    if (control)
    {
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(control), 0x149) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_002c), 0x14A) = 1;
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_002c), 0x1C6) = 0;
    }

    void* associated = LookupObjectById(
        GetGlobalInt(), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, associated, 0)[0x18 / 4])(associated, nullptr);

    associated = LookupObjectById(
        GetGlobalInt(), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, associated, 0)[0x18 / 4])(associated, nullptr);

    screen = reinterpret_cast<void*>(self->field_0284);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, self, 0)[0x44 / 4])(self, nullptr, screen);

    bool prepareNetworkAccount = false;
    if (g_MpAccountMode == 1 && g_MpNetworkType == 3)
    {
        void* nameEdit = reinterpret_cast<void*>(self->field_0020);
        if (!nameEdit)
        {
            prepareNetworkAccount = true;
        }
        else
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
                CSF_AT(void**, nameEdit, 0)[0x68 / 4])(
                    nameEdit, nullptr, 0, 1, 1);
        }
    }
    else
    {
        void* nameEdit = reinterpret_cast<void*>(self->field_0020);
        if (nameEdit)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
                CSF_AT(void**, nameEdit, 0)[0x68 / 4])(
                    nameEdit, nullptr, 1, 1, 1);
        }
    }

    if (prepareNetworkAccount || g_MpNetworkType == 3)
    {
        MenuMpAccounts_PrepareNetworkAccount();

        void* activeScreen = CSF_AT(
            void*, GetGlobalState_50D190(), 0x10);
        void* networkManager = GetGlobalState_6A6D70();
        void* screenMethod = CSF_AT(void**, activeScreen, 0)[0x64 / 4];
        void** networkVtable = CSF_AT(void**, networkManager, 0);

        int screenValue =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(screenMethod)(
                activeScreen, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            networkVtable[0x9C / 4])(networkManager, nullptr, screenValue);

        activeScreen = CSF_AT(
            void*, GetGlobalState_50D190(), 0x10);
        void* accountService = GetGlobalDword_73C020();
        MenuMpAccounts_SetAccountScreen(accountService, nullptr, activeScreen);
    }

    CMenuAccountEdit_ApplyHeldPositionDelta(self, nullptr);
    return static_cast<int>(self->field_0284);
}

HOOK(0x0055C490, CMenuAccountEdit_InitializeControls, "auto");
