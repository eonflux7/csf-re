#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, CMenuMpSessions_UpdateSessionPhase, 0x005607D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpSessions_RebuildPartidasList, 0x00560890, (CMenuMpSessions* self, void*));
CSF_FUNC(void, __fastcall, MenuMpSessions_DispatchState, 0x00561AF0, (void* self, void*));
CSF_FUNC(void*, __fastcall, CMenuMpSessions_BuildSessionEntry, 0x00563AB0, (CMenuMpSessions* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBox_GetItemCount_714860, 0x00714860, (CGuiListBox* self, void*));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(void*, __fastcall, GetContainerDataPtr, 0x00721270, (void* self, void*));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static int MenuMpSessions_GetControlState(std::uint32_t controlAddress)
{
    void* control = reinterpret_cast<void*>(controlAddress);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x2AC / 4])(control, nullptr);
}

static void MenuMpSessions_SetControlState(std::uint32_t controlAddress, int state)
{
    void* control = reinterpret_cast<void*>(controlAddress);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0x2A8 / 4])(control, nullptr, state);
}

static void CMenuMpSessions_InvokeAction(CMenuMpSessions* self)
{
    reinterpret_cast<void (__fastcall*)(CMenuMpSessions*, void*)>(
        CSF_AT(void**, self, 0)[0x54 / 4])(self, nullptr);
}

static int __fastcall CMenuMpSessions_HandleControlChange(
    CMenuMpSessions* self, void* /*edx*/, std::uint32_t* event)
{
    if (event[0] != 13)
        return static_cast<int>(event[0] - 13);

    const std::uint32_t changedControl = event[1];
    if (changedControl == self->field_0024)
    {
        if (MenuMpSessions_GetControlState(self->field_0028) &&
            MenuMpSessions_GetControlState(self->field_002c))
        {
            MenuMpSessions_SetControlState(self->field_0024, 0);
        }
    }
    else if (changedControl == self->field_0028)
    {
        if (MenuMpSessions_GetControlState(self->field_0024) &&
            MenuMpSessions_GetControlState(self->field_002c))
        {
            MenuMpSessions_SetControlState(self->field_0028, 0);
        }
    }
    else if (changedControl == self->field_002c)
    {
        if (MenuMpSessions_GetControlState(self->field_0028) &&
            MenuMpSessions_GetControlState(self->field_0024))
        {
            MenuMpSessions_SetControlState(self->field_002c, 0);
        }
    }

    switch (self->field_01e0)
    {
    case 0:
    {
        if (self->field_00a4 && self->field_00a4 == event[1])
            CMenuMpSessions_InvokeAction(self);

        void* container = reinterpret_cast<void*>(self->field_0040);
        const std::uint32_t activeControl =
            reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(
                CSF_AT(void**, container, 0)[0x104 / 4])(container, nullptr);

        auto* sessions = reinterpret_cast<CGuiListBox*>(self->field_0070);
        if (sessions &&
            (activeControl == reinterpret_cast<std::uint32_t>(sessions) ||
             activeControl == self->field_0068) &&
            CGuiListBox_GetItemCount_714860(sessions, nullptr) > 0)
        {
            const int sessionIndex = GetStoredValueAtIndex(
                reinterpret_cast<void*>(self->field_0070), nullptr, 0);
            if (sessionIndex >= 0)
                self->field_01cc = sessionIndex;
        }
        else
        {
            if (activeControl == self->field_004c)
                self->field_00b0 = 1;
            else if (activeControl == self->field_0050)
                self->field_00b1 = 1;
            else if (activeControl == self->field_0054)
                self->field_00b2 = 1;
            else if (activeControl == self->field_0058)
                self->field_00b3 = 1;
            else if (activeControl == self->field_005c)
                self->field_00b4 = 1;
            else if (activeControl == self->field_0060)
                self->field_00b5 = 1;
            else if (activeControl == self->field_0064)
                self->field_00b6 = 1;
        }
        return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
    }

    case 1:
        if (self->field_00a4 && self->field_00a4 == event[1])
        {
            CMenuMpSessions_InvokeAction(self);
            return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
        }
        break;

    case 3:
        break;

    default:
        return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
    }

    if (self->field_00ac && self->field_00ac == event[1])
    {
        CMenuMpSessions_UpdateSessionPhase(self, nullptr);
        return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
    }

    if (self->field_0078 && self->field_0078 == event[1])
    {
        const int state = MenuMpSessions_GetControlState(self->field_0078);
        self->field_001c = (state != 0) + 1;
        return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
    }

    if (self->field_0074 && self->field_0074 == event[1])
    {
        const int state = MenuMpSessions_GetControlState(self->field_0074);
        if (state == 0)
            self->field_0018 = 0;
        else if (state == 1)
            self->field_0018 = 1;
        else if (state == 2)
            self->field_0018 = 3;

        MenuMpSessions_DispatchState(self, nullptr);
        CMenuMpSessions_RebuildPartidasList(self, nullptr);
        return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
    }

    if ((self->field_0038 && self->field_0038 == event[1]) ||
        (self->field_0034 && self->field_0034 == event[1]))
    {
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
            void* entry = GetContainerDataPtr(
                reinterpret_cast<void*>(self->field_003c), nullptr);
            self->vGetSessionEntry(reinterpret_cast<int>(entry));
        }
        else
        {
            CMenuMpSessions_BuildSessionEntry(self, nullptr);
        }
    }

    return self->vHandleLinkedItemChange(reinterpret_cast<int>(event));
}

HOOK(0x00563FD0, CMenuMpSessions_HandleControlChange, "auto");
