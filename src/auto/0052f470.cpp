#include <cstdint>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, MenuScenarioSelect_GetSelectionContext,
         0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuScenarioSelect_SetContextSelection,
         0x005231D0, (int context, void* edx, int selection));
CSF_FUNC(void, __cdecl, MenuScenarioSelect_RunSelectionUpdateCallback,
         0x00506AC0, ());
CSF_FUNC(void, __fastcall, MenuScenarioSelect_UpdateSelectionControl,
         0x00542420, (int control, void* edx, float selection));
CSF_FUNC(int, __fastcall, CMenuMpLoadout_ForwardSelectionIfReady,
         0x0052EF10, (CMenuMpLoadout* self, void* edx));

static int __fastcall CMenuMpLoadout_SetSelection(
    CMenuMpLoadout* self, void* /*edx*/, float selection)
{
    std::uint32_t selectionBits;
    std::memcpy(&selectionBits, &selection, sizeof(selectionBits));
    self->field_003c = selectionBits;

    const int context = MenuScenarioSelect_GetSelectionContext();
    MenuScenarioSelect_SetContextSelection(
        context, nullptr, static_cast<int>(selectionBits));

    MenuScenarioSelect_RunSelectionUpdateCallback();

    reinterpret_cast<void (__fastcall*)(CMenuMpLoadout*, void*)>(
        CSF_AT(void**, self, 0)[0x50 / 4])(self, nullptr);

    MenuScenarioSelect_UpdateSelectionControl(
        static_cast<int>(self->field_0058), nullptr, selection);

    return CMenuMpLoadout_ForwardSelectionIfReady(self, nullptr);
}

HOOK(0x0052F470, CMenuMpLoadout_SetSelection, "auto");
