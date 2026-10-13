#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));

static int __fastcall CMenuMpOnline_ProcessMatchSelection(
    CMenuMpOnline* self, void* /*edx*/, int /*selection*/)
{
    void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    if (!reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr))
    {
        GameBddLoader_ResetPending(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

        state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0x94 / 4])(state, nullptr))
        {
            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            int entry = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0x20 / 4])(state, nullptr);
            switch (entry)
            {
            case 1229731124:
            {
                int result = GameBddLoader_DispatchMenuRequest(
                    reinterpret_cast<void*>(GetGlobalState_50D190()),
                    nullptr, 1480734007, 128, 4096, 0);
                self->field_0018 = 11;
                return result;
            }
            case 1229730104:
            {
                int result = GameBddLoader_DispatchMenuRequest(
                    reinterpret_cast<void*>(GetGlobalState_50D190()),
                    nullptr, 1480734257, 128, 6144, 0);
                self->field_0018 = 12;
                return result;
            }
            case 1229731894:
            {
                int result = GameBddLoader_DispatchMenuRequest(
                    reinterpret_cast<void*>(GetGlobalState_50D190()),
                    nullptr, 1480734006, 128, 6144, 0);
                self->field_0018 = 10;
                return result;
            }
            case 1229729841:
            case 1229731897:
                goto ShowMatchRequest;
            }
        }

        state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0x84 / 4])(state, nullptr))
        {
            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            int entry = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0x20 / 4])(state, nullptr);
            int result = GameBddLoader_DispatchMenuRequest(
                reinterpret_cast<void*>(GetGlobalState_50D190()),
                nullptr, entry, 32, 1, 0);
            self->field_0018 = 6;
            return result;
        }

    ShowMatchRequest:
        GameBddLoader_DispatchMenuRequest(
            reinterpret_cast<void*>(GetGlobalState_50D190()),
            nullptr, 1480734003, 128, 6144, 0);
        self->field_0018 = 9;
        state = reinterpret_cast<void*>(GetGlobalState_50D190());
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, state, 0)[0x68 / 4])(state, nullptr, 0);
    }

    state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr) == 2)
    {
        GameBddLoader_ResetPending(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0xFC / 4])(state, nullptr);
        if (result)
        {
            result = GameBddLoader_DispatchMenuRequest(
                reinterpret_cast<void*>(GetGlobalState_50D190()),
                nullptr, 1480734775, 128, 12, 0);
            self->field_0018 = 13;
        }
        else
        {
            self->field_0018 = 4;
        }
        return result;
    }

    state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr) == 8)
    {
        GameBddLoader_ResetPending(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        self->field_0018 = 8;
        return GameBddLoader_DispatchMenuRequest(
            reinterpret_cast<void*>(GetGlobalState_50D190()),
            nullptr, 1480734258, 128, 6144, 0);
    }

    state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x1C / 4])(state, nullptr) == 11)
    {
        GameBddLoader_ResetPending(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        int entry = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x20 / 4])(state, nullptr);

        int requestData;
        switch (entry)
        {
        case 1480734004:
            requestData = 6144;
            break;
        case 1480734005:
            requestData = 4096;
            break;
        default:
            return static_cast<int>(
                static_cast<uint32_t>(entry) - 1480734004 - 1);
        }

        int result = GameBddLoader_DispatchMenuRequest(
            reinterpret_cast<void*>(GetGlobalState_50D190()),
            nullptr, entry, 128, requestData, 0);
        self->field_0018 = 14;
        return result;
    }

    void* scheduler = C_00861120_GetSchedulerProxy(
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190()), nullptr);
    if (!scheduler)
        return 0;

    GameBddLoader_DispatchMenuRequest(
        reinterpret_cast<void*>(GetGlobalState_50D190()),
        nullptr, 1279341107, 1, 0, 0);
    state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, state, 0)[0x40 / 4])(state, nullptr);
    self->field_0018 = 5;
    return result;
}

HOOK(0x0055F0B0, CMenuMpOnline_ProcessMatchSelection, "auto");
