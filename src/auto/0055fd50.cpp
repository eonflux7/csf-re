#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, MenuMpOnline_ActivateMenuControls, 0x0055F7E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

CSF_GLOBAL(int, g_SelectedAccountIndex, 0x0089A5E8);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

// The packet also lists the direct-call address as a vtable reference.
[[maybe_unused]] static const auto kMenuControlsVtable = CSF_VTBL(0x0055F7E0);

static int MenuMpOnline_GetMatchAvailability()
{
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        return g_OnlineMatchFlag;
    }
    return g_MpNetworkType == 3;
}

static int __fastcall S_0055f7e0_ProcessOnlineState(
    S_0055f7e0* self, void* /*edx*/, int /*unused*/)
{
    int result;

    if (self->field_0048)
    {
        result = MenuMpOnline_GetMatchAvailability();
        if (result)
        {
            alignas(8) std::uint8_t menuBuffer[0xB4];
            MenuBuffer_Init(menuBuffer, nullptr);
            menuBuffer[24] = 1;

            void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_DispatchMenuRequest(
                loader, nullptr, 1279341104, 1, 0,
                reinterpret_cast<int>(menuBuffer));
            result = static_cast<int>(SceneLoader_DestroyMenuBuffer(
                reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr));
        }
        self->field_0048 = 0;
        return result;
    }

    MenuMpOnline_ActivateMenuControls(self, nullptr);
    result = MenuMpOnline_GetMatchAvailability();

    if (result)
    {
        void* globalValue = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* accounts = CSF_AT(void*, globalValue, 0x14);
        int accountValue =
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, accounts, 0)[0x30 / 4])(
                    accounts, nullptr, g_SelectedAccountIndex);

        if (accountValue)
        {
            self->field_0018 = 3;
            void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            auto* account = reinterpret_cast<std::uint8_t*>(accountValue);
            return reinterpret_cast<int (__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, state, 0)[0x3C / 4])(
                    state, nullptr, account + 8, account + 41);
        }

        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = GameBddLoader_DispatchMenuRequest(
            loader, nullptr, 1280323638, 1, 1, 0);
        self->field_0018 = 6;
    }
    else
    {
        void* control = reinterpret_cast<void*>(self->field_0028);
        if (control)
        {
            result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, control, 0)[0xA8 / 4])(
                    control, nullptr, 1);
        }
        self->field_0018 = 4;
    }

    return result;
}

HOOK(0x0055FD50, S_0055f7e0_ProcessOnlineState, "auto");
