#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __stdcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (int a1));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_RefreshPanelAtIndex, 0x0053E2F0, (CMenuMissionInfo* self, void* edx, int index));
CSF_FUNC(void, __fastcall, CMenuMissionInfo_ProcessPendingPageStep, 0x0053E9A0, (CMenuMissionInfo* self, void*));
CSF_FUNC(char, __fastcall, CMenuMissionInfo_TryAdvanceToNextPage, 0x0053E9E0, (void* self, void*));

static void* MenuMissionInfo_FindControl(void* parent, const char* name)
{
    return reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, parent, 0)[0xFC / 4])(parent, nullptr, name);
}

static bool MenuMissionInfo_IsPanelReady(void* panel)
{
    return reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, panel, 0)[0xAC / 4])(panel, nullptr);
}

static void MenuMissionInfo_SetPanelHidden(void* panel, bool hidden)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, bool)>(
        CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, hidden);
}

static char __fastcall CMenuMissionInfo_DispatchCommand(
    CMenuMissionInfo* self, void* /*edx*/, void* command)
{
    void* root = reinterpret_cast<void*>(self->field_001c);
    int state = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, root, 0)[0x104 / 4])(root, nullptr);
    if (!state)
        return static_cast<char>(state);

    void* tabs = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842EC8, "GRadio"));
    if (!tabs)
        return 0;

    void* tacticalMap = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842FE4, "PanelMapaTactico"));
    void* objectives = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842F9C, "PanelObjetivos"));
    void* characters = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842CD4, "PanelBichos"));
    void* killsReport = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842F68, "PanelKillsReport"));
    void* overall = MenuMissionInfo_FindControl(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842EF0, "PanelOverall"));

    if (!tacticalMap || !objectives || !characters || !killsReport || !overall)
        return static_cast<char>(reinterpret_cast<uintptr_t>(overall));

    bool selectMap = false;
    if (MenuMissionInfo_IsPanelReady(tacticalMap))
    {
        void* source = CSF_AT(void*, command, 0x4);
        selectMap = source == MenuMissionInfo_FindControl(
            tabs, CSF_STR(0x00842EAC, "BotonPestaniaMapaTactico"));
    }

    bool selectObjectives = false;
    if (MenuMissionInfo_IsPanelReady(objectives))
    {
        void* source = CSF_AT(void*, command, 0x4);
        selectObjectives = source == MenuMissionInfo_FindControl(
            tabs, CSF_STR(0x00842E94, "BotonPestaniaObjetivos"));
    }

    bool selectCharacters = false;
    if (MenuMissionInfo_IsPanelReady(characters))
    {
        void* source = CSF_AT(void*, command, 0x4);
        selectCharacters = source == MenuMissionInfo_FindControl(
            tabs, CSF_STR(0x0084302C, "BotonPestaniaBichos"));
    }

    bool selectKillsReport = false;
    if (MenuMissionInfo_IsPanelReady(killsReport))
    {
        void* source = CSF_AT(void*, command, 0x4);
        selectKillsReport = source == MenuMissionInfo_FindControl(
            tabs, CSF_STR(0x00843010, "BotonPestaniaKillsReport"));
    }

    bool selectOverall = false;
    if (MenuMissionInfo_IsPanelReady(overall))
    {
        void* source = CSF_AT(void*, command, 0x4);
        selectOverall = source == MenuMissionInfo_FindControl(
            tabs, CSF_STR(0x00842FF8, "BotonPestaniaOverall"));
    }

    void* source = CSF_AT(void*, command, 0x4);
    bool previousPage = source == MenuMissionInfo_FindControl(
        characters, CSF_STR(0x00842D0C, "BotonPanelAnterior"));
    source = CSF_AT(void*, command, 0x4);
    bool nextPage = source == MenuMissionInfo_FindControl(
        characters, CSF_STR(0x00842CF8, "BotonPanelSiguiente"));

    if (!(selectMap || selectObjectives || selectKillsReport ||
          selectOverall || selectCharacters || previousPage || nextPage))
        return static_cast<char>(selectMap);

    void* ownedPanel = reinterpret_cast<void*>(self->field_0084);
    reinterpret_cast<void(__fastcall*)(void*, void*, bool)>(
        CSF_AT(void**, ownedPanel, 0)[0xA8 / 4])(
            ownedPanel, nullptr, !selectMap);
    MenuMissionInfo_SetPanelHidden(tacticalMap, !selectMap);
    MenuMissionInfo_SetPanelHidden(objectives, !selectObjectives);
    MenuMissionInfo_SetPanelHidden(killsReport, !selectKillsReport);
    MenuMissionInfo_SetPanelHidden(overall, !selectOverall);
    MenuMissionInfo_SetPanelHidden(
        characters, !selectCharacters && !previousPage && !nextPage);

    if (previousPage)
    {
        CMenuMissionInfo_ProcessPendingPageStep(self, nullptr);
        return 0;
    }
    if (nextPage)
        return CMenuMissionInfo_TryAdvanceToNextPage(self, nullptr);

    if (selectCharacters)
    {
        self->field_0088 = 0;
        CMenuMissionInfo_RefreshPanelAtIndex(self, nullptr, 0);
    }
    Get_008b0678();
    return static_cast<char>(MenuMissionInfo_SetGlobalIndex(97));
}

HOOK(0x00540030, CMenuMissionInfo_DispatchCommand, "auto");
