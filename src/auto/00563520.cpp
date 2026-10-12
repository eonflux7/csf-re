#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(void, __fastcall, CMenuMpSessions_RefreshOptionControls, 0x00560960, (CMenuMpSessions* self, void*));
CSF_FUNC(int, __fastcall, MenuMpSessions_HandleState1Or3, 0x00560BD0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpSessions_SetControlsActive, 0x00560F80, (CMenuMpSessions* self, void*));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_RefreshOptionState, 0x00562770, (CMenuMpSessions* self, void* edx, char a3, char a4));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_HandleStartAction, 0x00562D50, (uint32_t* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_HandleSelectedSession, 0x00562ED0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, ForwardToSub6687B0AtOffset9c, 0x0073D4C0, (void* self, void*));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int __fastcall CMenuMpSessions_ProcessPendingActions(
    CMenuMpSessions* self, void* /*edx*/)
{
    bool optionsChanged = false;
    int result = 0;

    if (self->field_00b0)
    {
        switch (self->field_0018)
        {
        case 0: self->field_0018 = 1; break;
        case 1: self->field_0018 = 3; break;
        case 3: self->field_0018 = 5; break;
        case 5: self->field_0018 = 0; break;
        default: break;
        }
        optionsChanged = true;
    }
    else if (self->field_00b1)
    {
        switch (self->field_01b8)
        {
        case 0: self->field_01b8 = 1; break;
        case 1: self->field_01b8 = 2; break;
        case 2: self->field_01b8 = 0; break;
        default: break;
        }
        optionsChanged = true;
    }
    else if (self->field_00b2)
    {
        switch (self->field_01bc)
        {
        case 0: self->field_01bc = 1; break;
        case 1: self->field_01bc = 2; break;
        case 2: self->field_01bc = 0; break;
        default: break;
        }
        optionsChanged = true;
    }
    else if (self->field_00b3)
    {
        switch (self->field_001c)
        {
        case 0: self->field_001c = 1; break;
        case 1: self->field_001c = 2; break;
        case 2: self->field_001c = 0; break;
        default: break;
        }
        optionsChanged = true;
    }
    else if (self->field_00b4)
    {
        switch (self->field_01c0)
        {
        case 0: self->field_01c0 = 1; break;
        case 1: self->field_01c0 = 2; break;
        case 2: self->field_01c0 = 0; break;
        default: break;
        }
        optionsChanged = true;
    }
    else if (self->field_00b5)
    {
        result = static_cast<int>(CMenuMpSessions_HandleStartAction(
            reinterpret_cast<uint32_t*>(self), 0));
    }
    else if (self->field_00b6)
    {
        int online;
        if (GetGlobal8B0688())
        {
            GetGlobal8B0688();
            online = g_OnlineMatchFlag;
        }
        else
        {
            online = g_MpNetworkType == 3;
        }

        if (online)
        {
            void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, state, 0)[0xCC / 4])(state, nullptr);
        }
        else
        {
            void* network = reinterpret_cast<void*>(GetGlobalDword_73C020());
            ForwardToSub6687B0AtOffset9c(network, nullptr);
        }
        result = CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 1);
    }
    else if (self->field_01cc != static_cast<uint32_t>(-1))
    {
        result = static_cast<int>(
            CMenuMpSessions_HandleSelectedSession(self, nullptr));
    }

    self->field_00b0 = 0;
    self->field_00b1 = 0;
    self->field_00b2 = 0;
    self->field_00b3 = 0;
    self->field_00b4 = 0;
    self->field_00b5 = 0;
    self->field_00b6 = 0;

    if (optionsChanged)
    {
        const uint32_t state = self->field_01e0;
        switch (state)
        {
        case 0:
            CMenuMpSessions_RefreshOptionControls(self, nullptr);
            return 0;
        case 1:
        case 3:
            return MenuMpSessions_HandleState1Or3(self, nullptr);
        case 2:
        case 4:
            return CMenuMpSessions_SetControlsActive(self, nullptr);
        default:
            return static_cast<int>(state);
        }
    }
    return result;
}

HOOK(0x00563520, CMenuMpSessions_ProcessPendingActions, "auto");
