#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpLoadout_BindChildControls, 0x0052ED00, (CMenuMpLoadout* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpLoadout_UpdateSelection, 0x0052EDE0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, CMenuPause_CenterOnTarget, 0x0052F950, (CMenuPause* self, void*));
CSF_FUNC(CGuiCharacterPreview*, __fastcall, CGuiCharacterPreview_Construct, 0x00541E70, (CGuiCharacterPreview* self, void*));
CSF_FUNC(int, __fastcall, MenuMpCharacterSelect_CreatePreview, 0x00542070, (void* self, void* , void* control));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

static uint32_t __fastcall CMenuMpLoadout_InitializeScreen(
    CMenuMpLoadout* self, void* /*edx*/, uint32_t context, int /*unused*/)
{
    self->field_0040 = static_cast<uint32_t>(-1);
    self->field_0048 = static_cast<uint32_t>(-1);
    self->field_0050 = static_cast<uint32_t>(-1);
    self->field_0014 = 0;
    self->field_003c = 0;
    self->field_0044 = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        self->vtbl[0x40 / 4])(self, nullptr);

    self->field_0014 = context;

    auto* storage = reinterpret_cast<CGuiCharacterPreview*>(
        CoreMemory_AllocUntagged(344));
    CGuiCharacterPreview* preview = storage
        ? CGuiCharacterPreview_Construct(storage, nullptr)
        : nullptr;

    void* control = reinterpret_cast<void*>(self->field_0018);
    self->field_0058 = reinterpret_cast<uint32_t>(preview);
    MenuMpCharacterSelect_CreatePreview(preview, nullptr, control);

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(globalState, nullptr);

    control = reinterpret_cast<void*>(self->field_0018);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x16c / 4])(control, nullptr);

    CMenuMpLoadout_BindChildControls(self, nullptr);
    CMenuMpLoadout_UpdateSelection(
        self, nullptr, static_cast<int>(self->field_0040));

    GetGlobal8B0688();
    if (self->field_0050 != static_cast<uint32_t>(GetMpGameModeIndex()))
    {
        GetGlobal8B0688();
        self->field_0050 = static_cast<uint32_t>(GetMpGameModeIndex());
    }

    CMenuPause_CenterOnTarget(reinterpret_cast<CMenuPause*>(self), nullptr);

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        self->vtbl[0x44 / 4])(self, nullptr, self->field_0018);

    return self->field_0018;
}

HOOK(0x0052F360, CMenuMpLoadout_InitializeScreen, "auto");
