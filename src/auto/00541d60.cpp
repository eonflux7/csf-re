#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpStats_UpdateStatistics, 0x00541A30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x00714750, (CGuiListBoxBase* self, void* , int itemHeight));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));
CSF_FUNC(void*, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x00722DF0, (void* self, void* edx));

static int __fastcall CMenuMpStats_InitializeStatistics(
    CMenuMpStats* self, void* /*edx*/, int context, int /*unused*/)
{
    void* screen = reinterpret_cast<void*>(self->field_0084);
    self->field_0080 = context;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[364 / 4])(screen, nullptr);

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    screen = reinterpret_cast<void*>(self->field_0084);
    std::uint32_t listBox = reinterpret_cast<std::uint32_t(__fastcall*)(
        void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[252 / 4])(
            screen, nullptr, CSF_STR(0x00843270, "ListboxEstadisticas"));

    screen = reinterpret_cast<void*>(self->field_0084);
    self->field_0010 = listBox;
    std::uint32_t team1Text = reinterpret_cast<std::uint32_t(__fastcall*)(
        void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[252 / 4])(
            screen, nullptr, CSF_STR(0x008431D4, "TextoTeam1"));

    screen = reinterpret_cast<void*>(self->field_0084);
    self->field_0014 = team1Text;
    std::uint32_t team2Text = reinterpret_cast<std::uint32_t(__fastcall*)(
        void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[252 / 4])(
            screen, nullptr, CSF_STR(0x008431C8, "TextoTeam2"));

    CGuiListBoxBase* statistics =
        reinterpret_cast<CGuiListBoxBase*>(self->field_0010);
    self->field_0018 = team2Text;
    if (statistics)
    {
        void* metrics = ForwardToStoredObjectVirtualSlot40(statistics, nullptr);
        if (metrics)
        {
            int itemHeight = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, metrics, 0)[24 / 4])(metrics, nullptr, 1);
            CGuiListBoxBase_SetItemHeight(
                reinterpret_cast<CGuiListBoxBase*>(self->field_0010),
                nullptr, itemHeight);
        }

        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 0, 300);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 1, 100);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 2, 100);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 3, 100);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 4, 100);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 5, 100);
        CGuiListBoxBase_SetColumnWidth(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0010), nullptr, 6, 100);

        CMenuMpStats_UpdateStatistics(self, nullptr, 1);
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    C_00830578_ResetMenuController(
        CSF_AT(C_00830578*, globalState, 0x18), nullptr);
    return static_cast<int>(self->field_0084);
}

HOOK(0x00541D60, CMenuMpStats_InitializeStatistics, "auto");
