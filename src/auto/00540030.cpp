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

static void* MenuMissionInfo_FindChild(void* object, const char* name)
{
    return reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, object, 0)[0xFC / 4])(object, nullptr, name);
}

static bool MenuMissionInfo_IsPanelReady(void* panel)
{
    return reinterpret_cast<bool (__fastcall*)(void*, void*)>(
        CSF_AT(void**, panel, 0)[0xAC / 4])(panel, nullptr);
}

static void MenuMissionInfo_SetPanelHidden(void* panel, bool hidden)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, bool)>(
        CSF_AT(void**, panel, 0)[0xA8 / 4])(panel, nullptr, hidden);
}

static bool MenuMissionInfo_MatchesReadyTab(
    void* panel, void* radio, void* command, const char* tabName)
{
    if (!MenuMissionInfo_IsPanelReady(panel))
        return false;

    void* source = CSF_AT(void*, command, 0x4);
    void* tab = MenuMissionInfo_FindChild(radio, tabName);
    return source == tab;
}

static char __fastcall CMenuMissionInfo_DispatchCommand(
    CMenuMissionInfo* self, void* /*edx*/, void* command)
{
    void* root = reinterpret_cast<void*>(self->field_001c);
    int state = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, root, 0)[0x104 / 4])(root, nullptr);
    if (!state)
        return static_cast<char>(state);

    void* radio = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842EC8, "GRadio"));
    if (!radio)
        return 0;

    void* mapPanel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842FE4, "PanelMapaTactico"));
    void* objectivesPanel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842F9C, "PanelObjetivos"));
    void* creaturesPanel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842CD4, "PanelBichos"));
    void* killsPanel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842F68, "PanelKillsReport"));
    void* overallPanel = MenuMissionInfo_FindChild(
        reinterpret_cast<void*>(self->field_001c),
        CSF_STR(0x00842EF0, "PanelOverall"));

    if (!mapPanel || !objectivesPanel || !creaturesPanel ||
        !killsPanel || !overallPanel)
    {
        return static_cast<char>(reinterpret_cast<uintptr_t>(overallPanel));
    }

    bool mapTab = MenuMissionInfo_MatchesReadyTab(
        mapPanel, radio, command,
        CSF_STR(0x00842EAC, "BotonPestaniaMapaTactico"));
    bool objectivesTab = MenuMissionInfo_MatchesReadyTab(
        objectivesPanel, radio, command,
        CSF_STR(0x00842E94, "BotonPestaniaObjetivos"));
    bool creaturesTab = MenuMissionInfo_MatchesReadyTab(
        creaturesPanel, radio, command,
        CSF_STR(0x0084302C, "BotonPestaniaBichos"));
    bool killsTab = MenuMissionInfo_MatchesReadyTab(
        killsPanel, radio, command,
        CSF_STR(0x00843010, "BotonPestaniaKillsReport"));
    bool overallTab = MenuMissionInfo_MatchesReadyTab(
        overallPanel, radio, command,
        CSF_STR(0x00842FF8, "BotonPestaniaOverall"));

    void* previousSource = CSF_AT(void*, command, 0x4);
    void* previousButton = MenuMissionInfo_FindChild(
        creaturesPanel, CSF_STR(0x00842D0C, "BotonPanelAnterior"));
    bool previousPage = previousSource == previousButton;

    void* nextSource = CSF_AT(void*, command, 0x4);
    void* nextButton = MenuMissionInfo_FindChild(
        creaturesPanel, CSF_STR(0x00842CF8, "BotonPanelSiguiente"));
    bool nextPage = nextSource == nextButton;

    if (!mapTab && !objectivesTab && !killsTab && !overallTab &&
        !creaturesTab && !previousPage && !nextPage)
    {
        return 0;
    }

    MenuMissionInfo_SetPanelHidden(
        reinterpret_cast<void*>(self->field_0084), !mapTab);
    MenuMissionInfo_SetPanelHidden(mapPanel, !mapTab);
    MenuMissionInfo_SetPanelHidden(objectivesPanel, !objectivesTab);
    MenuMissionInfo_SetPanelHidden(killsPanel, !killsTab);
    MenuMissionInfo_SetPanelHidden(overallPanel, !overallTab);
    MenuMissionInfo_SetPanelHidden(
        creaturesPanel, !creaturesTab && !previousPage && !nextPage);

    if (previousPage)
    {
        CMenuMissionInfo_ProcessPendingPageStep(self, nullptr);
        // The bound callee is void; its incidental AL is not a defined result.
        return 0;
    }
    if (nextPage)
        return CMenuMissionInfo_TryAdvanceToNextPage(self, nullptr);

    if (creaturesTab)
    {
        self->field_0088 = 0;
        CMenuMissionInfo_RefreshPanelAtIndex(self, nullptr, 0);
    }

    Get_008b0678();
    return static_cast<char>(MenuMissionInfo_SetGlobalIndex(97));
}

HOOK(0x00540030, CMenuMissionInfo_DispatchCommand, "auto");
