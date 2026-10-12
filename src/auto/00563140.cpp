#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __fastcall, CMenuMpSessions_InitializeControls, 0x00562000, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_RefreshOptionState, 0x00562770, (CMenuMpSessions* self, void* edx, char a3, char a4));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ForwardToSub6687B0AtOffset9c, 0x0073D4C0, (void* self, void*));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static std::uint32_t MenuMpSessions_FindControl(void* root, const char* name)
{
    return reinterpret_cast<std::uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, root, 0)[0xFC / 4])(root, nullptr, name);
}

static int __fastcall CMenuMpSessions_InitializeSessionOptions(
    CMenuMpSessions* self, void* /*edx*/, int /*a3*/, int /*a4*/)
{
    CMenuMpSessions_InitializeControls(self, nullptr);

    void* root = reinterpret_cast<void*>(self->field_0040);
    std::uint32_t control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A78, "ButTipo"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_004c = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A6C, "ButFriends"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0050 = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A5C, "ButBlackList"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0054 = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A4C, "ButFriendlyFire"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0058 = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A3C, "stFuegoAmigo"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_006c = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A30, "ButPrivado"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_005c = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A20, "ButNewSearch"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0060 = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A14, "ButClear"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0064 = control;

    control = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845A0C, "ButJoin"));
    root = reinterpret_cast<void*>(self->field_0040);
    self->field_0068 = control;

    self->field_00a4 = MenuMpSessions_FindControl(
        root, CSF_STR(0x00845904, "ButSave"));

    bool onlineMatch;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        onlineMatch = g_OnlineMatchFlag != 0;
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

    CGuiSlider* newSearch = reinterpret_cast<CGuiSlider*>(self->field_0060);
    if (newSearch)
        CGuiSlider_ForwardSelfToParent(newSearch, nullptr);

    int result = CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 1);
    self->field_01e4 = 0;
    return result;
}

HOOK(0x00563140, CMenuMpSessions_InitializeSessionOptions, "auto");
