#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, CMenuMpSessions_RebuildPartidasList, 0x00560890, (CMenuMpSessions* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpSessions_PrepareControls, 0x00562000, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_RefreshOptionState, 0x00562770, (CMenuMpSessions* self, void* edx, char a3, char a4));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ForwardToSub6687B0AtOffset9c, 0x0073D4C0, (void* self, void*));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall CMenuMpSessions_InitializeSessionOptions(
    CMenuMpSessions* self, void* /*edx*/, int /*unused1*/, int /*unused2*/)
{
    CMenuMpSessions_PrepareControls(self, nullptr);

    void* control = reinterpret_cast<void*>(self->field_0070);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 0, 1, 1);

    control = reinterpret_cast<void*>(self->field_0034);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 0, 1, 1);

    void* container = reinterpret_cast<void*>(self->field_0040);
    self->field_01e8 = 0;
    self->field_01e9 = 0;
    self->field_01ec = 0;
    self->field_01f0 = 1;

    self->field_006c =
        reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, container, 0)[0xFC / 4])(
                container, nullptr, CSF_STR(0x00845A3C, "stFuegoAmigo"));

    int onlineMatch;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        onlineMatch = g_OnlineMatchFlag;
    }
    else
    {
        onlineMatch = g_MpNetworkType == 3;
    }

    if (onlineMatch)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0xCC / 4])(state, nullptr);
    }
    else
    {
        void* state = reinterpret_cast<void*>(GetGlobalDword_73C020());
        ForwardToSub6687B0AtOffset9c(state, nullptr);
    }

    CMenuMpSessions_RebuildPartidasList(self, nullptr);

    const std::uint32_t currentValue = self->field_0020;
    self->field_01e4 = 1;
    self->field_0014 = 0;
    self->field_0018 = currentValue;

    return CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 1);
}

HOOK(0x00563280, CMenuMpSessions_InitializeSessionOptions, "auto");
