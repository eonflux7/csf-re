#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static std::uint32_t __fastcall CMenuMpOnline_ProcessInitialState(
    CMenuMpOnline* self, void* /*edx*/, int /*argument*/)
{
    std::uint32_t result;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        result = g_OnlineMatchFlag;
    }
    else
    {
        result = g_MpNetworkType == 3;
    }

    if (result)
    {
        alignas(8) uint8_t menuBuffer[0xB4];
        MenuBuffer_Init(menuBuffer, nullptr);
        CSF_AT(std::uint32_t, menuBuffer, 0x14) = 1082130432;
        CSF_AT(CMenuMpOnline*, menuBuffer, 0x10) = self;
        CSF_AT(std::uint8_t, menuBuffer, 0x18) = 1;

        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            globalState, nullptr, 1397961013, 1, 0,
            static_cast<int>(reinterpret_cast<std::intptr_t>(menuBuffer)));

        self->field_0018 = 1;
        return SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
    }

    self->field_0018 = 2;
    return result;
}

HOOK(0x0055FCB0, CMenuMpOnline_ProcessInitialState, "auto");
