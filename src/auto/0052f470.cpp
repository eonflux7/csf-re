#include <cstdint>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0,
         0x00521BB0, ());
CSF_FUNC(void, __fastcall, GlobalSetValue_5231D0,
         0x005231D0, (int context, void* edx, int selection));
CSF_FUNC(void, __cdecl, Nullsub_506AC0,
         0x00506AC0, ());
CSF_FUNC(void, __fastcall, MenuMpCharacterSelect_UpdatePreview,
         0x00542420, (int control, void* edx, float selection));
CSF_FUNC(int, __fastcall, CMenuMpLoadout_ForwardSelectionIfReady,
         0x0052EF10, (CMenuMpLoadout* self, void* edx));

static int __fastcall CMenuMpLoadout_SetSelection(
    CMenuMpLoadout* self, void* /*edx*/, float selection)
{
    std::uint32_t selectionBits;
    std::memcpy(&selectionBits, &selection, sizeof(selectionBits));
    self->field_003c = selectionBits;

    const int context = GetGlobalValue_521BB0();
    GlobalSetValue_5231D0(
        context, nullptr, static_cast<int>(selectionBits));

    Nullsub_506AC0();

    reinterpret_cast<void (__fastcall*)(CMenuMpLoadout*, void*)>(
        CSF_AT(void**, self, 0)[0x50 / 4])(self, nullptr);

    MenuMpCharacterSelect_UpdatePreview(
        static_cast<int>(self->field_0058), nullptr, selection);

    return CMenuMpLoadout_ForwardSelectionIfReady(self, nullptr);
}

HOOK(0x0052F470, CMenuMpLoadout_SetSelection, "auto");
