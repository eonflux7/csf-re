#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, S_00536500_RefreshSelectionState, 0x00536500, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00536500_CompleteSelection, 0x00536CD0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, MenuMpCharacterSelect_TickCameraPreview, 0x005425D0, ());
CSF_FUNC(int, __stdcall, MenuMpCharacterSelect_UpdatePreviewIfActive, 0x005425F0, (float a1));
CSF_FUNC(bool, __cdecl, MenuMPCharacterSelect_HasStateChanged, 0x00542620, ());
CSF_FUNC(int, __stdcall, MenuMpCharacterSelect_DispatchValueToGlobal, 0x00542730, (int value));
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_IsActive, 0x005427C0, ());

// Explicitly bind the vtable reference listed in the packet.
[[maybe_unused]] static const auto kSelectionStateVtable = CSF_VTBL(0x00536500);

static int __fastcall S_00536500_UpdateCharacterSelection(
    S_00536500* self, void* /*edx*/, int value)
{
    int globalState = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(globalState), 0xD0))
        return globalState;

    globalState = GetGlobalState_50D190();
    MenuMpCharacterSelect_UpdatePreviewIfActive(
        CSF_AT(float, reinterpret_cast<void*>(globalState), 0xA0));
    MenuMpCharacterSelect_TickCameraPreview();
    SceneLoader_NoOp();
    MenuMpCharacterSelect_DispatchValueToGlobal(value);

    if (MenuMPCharacterSelect_HasStateChanged())
    {
        globalState = GetGlobalState_50D190();
        CSF_AT(int, reinterpret_cast<void*>(globalState), 0x10C) = 10;
        S_00536500_RefreshSelectionState(self, nullptr);
    }

    const uint32_t selectionState = self->field_0148;
    switch (selectionState)
    {
    case 0:
        reinterpret_cast<void(__fastcall*)(S_00536500*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0xA8 / 4])(
                self, nullptr);
        CSF_AT(uint8_t, self, 0x140) = 1;
        self->field_0148 = 1;
        return 1;

    case 1:
        if (static_cast<uint8_t>(MenuMpCharacterSelect_IsActive()))
            return S_00536500_CompleteSelection(self, nullptr);
        return reinterpret_cast<int(__fastcall*)(S_00536500*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0xB4 / 4])(
                self, nullptr);

    case 2:
    {
        int result = self->field_0272;
        if (result)
            return result;

        result = MenuMpCharacterSelect_IsActive();
        if (static_cast<uint8_t>(result))
            return S_00536500_CompleteSelection(self, nullptr);
        return result;
    }

    default:
        return static_cast<int>(selectionState - 2);
    }
}

HOOK(0x00536D30, S_00536500_UpdateCharacterSelection, "auto");
