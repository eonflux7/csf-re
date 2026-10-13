#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void**, __fastcall, CMenuPlayerList_RefreshOnlinePlayers, 0x0055E340, (void* self, void* edx));
CSF_FUNC(void**, __fastcall, MenuMpAccounts_RefreshPlayerList, 0x0055E7B0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(std::uint8_t, g_bMenuFlag_8B0902, 0x008B0902);

static void** __fastcall CMenuPlayerList_UpdatePlayerList(
    CMenuPlayerList* self, void* /*edx*/, int deltaBits)
{
    void* child = reinterpret_cast<void*>(self->field_001c);
    if (child &&
        !((bool(__fastcall*)(void*, void*))
              CSF_AT(void**, child, 0)[0xBC / 4])(child, nullptr))
    {
        child = reinterpret_cast<void*>(self->field_001c);
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, child, 0)[0xB8 / 4])(child, nullptr, 1);
    }

    auto* timeState =
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0());
    S_00522010_AccumulateAndPropagateTime(timeState, nullptr, deltaBits);
    SceneLoader_NoOp();

    if (self->field_002c == 1)
    {
        void* manager = reinterpret_cast<void*>(GetGlobal8B0688());
        void* readinessObject = CSF_AT(void*, manager, 0x1484);
        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, readinessObject, 0)[0x14 / 4])(
                readinessObject, nullptr))
        {
            return nullptr;
        }

        if (g_bMenuFlag_8B0902 && g_OnlineMatchFlag)
        {
            void* onlineState =
                reinterpret_cast<void*>(GetGlobalState_6A6D70());
            void* matchState =
                ((void*(__fastcall*)(void*, void*))
                     CSF_AT(void**, onlineState, 0)[0xF4 / 4])(
                    onlineState, nullptr);

            if (!((bool(__fastcall*)(void*, void*))
                      CSF_AT(void**, matchState, 0)[0x3C / 4])(
                    matchState, nullptr))
            {
                return nullptr;
            }
        }

        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_ResetPending(loader, nullptr);

        void** result;
        if (g_bMenuFlag_8B0902 && g_OnlineMatchFlag)
            result = CMenuPlayerList_RefreshOnlinePlayers(self, nullptr);
        else
            result = MenuMpAccounts_RefreshPlayerList(self, nullptr);

        self->field_002c = 0;
        return result;
    }

    if (g_bMenuFlag_8B0902 && g_OnlineMatchFlag)
        return CMenuPlayerList_RefreshOnlinePlayers(self, nullptr);

    return MenuMpAccounts_RefreshPlayerList(self, nullptr);
}

HOOK(0x0055E980, CMenuPlayerList_UpdatePlayerList, "auto");
