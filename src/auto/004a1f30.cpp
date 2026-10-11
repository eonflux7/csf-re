#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, HudAlertIcons_IsFieldAboveZero, 0x004A1960, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, HudAlertIcons_CopySelectionHandle, 0x004CFAB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));

CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall HudAlertIcons_EvaluateStageThree(void* self, void* /*edx*/)
{
    void* state = CSF_AT(void*, self, 0x08);
    if (CSF_AT(int, state, 0x298) != 0 ||
        CSF_AT(int, state, 0x248) < 0)
        return;

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (!CSF_AT(uint8_t, globalState, 0xD0))
        return;

    void* control = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(
            static_cast<uint8_t*>(CSF_AT(void*, self, 0x08)) + 0x240)));
    if (!control || CSF_AT(int, control, 0x50) != 32)
        return;

    state = CSF_AT(void*, self, 0x08);
    if (!(CSF_AT(float, state, 0x258) <= g_CharacterFacingDistance))
        return;

    void* selection = CSF_AT(void*, control, 0x34);
    uint8_t* selectedBase = CSF_AT(uint8_t*, control, 0x18);
    void* selectedCharacter = selectedBase ? selectedBase - 128 : nullptr;
    uint8_t* ownerBase = CSF_AT(uint8_t*, state, 0x18);
    void* ownerCharacter = ownerBase ? ownerBase - 1712 : nullptr;

    if (!selection || !ownerCharacter || !selectedCharacter)
        return;
    if (static_cast<uint8_t>(
            HudAlertIcons_IsFieldAboveZero(selectedCharacter, nullptr)) != 0)
        return;
    if (CSF_AT(uint8_t, selectedCharacter, 0x50) !=
        CSF_AT(uint8_t, ownerCharacter, 0x443))
        return;

    alignas(8) uint8_t handleBytes[0x10];
    auto* handle = reinterpret_cast<uint32_t*>(handleBytes);

    if (!CSF_AT(uint8_t, selectedCharacter, 0x44))
    {
        uint32_t* result =
            HudAlertIcons_CopySelectionHandle(selection, nullptr, handle);
        const char* description = result[1] == UINT32_MAX
            ? CSF_STR(0x008344C8, "G075")
            : CSF_STR(0x008344C0, "G076");
        int descriptionId = MenuExtras_ParseDescriptionId(description);
        CSF_AT(int, CSF_AT(void*, self, 0x08), 0x2AC) = descriptionId;
        return;
    }

    uint32_t* result =
        HudAlertIcons_CopySelectionHandle(selection, nullptr, handle);
    if (result[1] != UINT32_MAX)
    {
        int descriptionId =
            MenuExtras_ParseDescriptionId(CSF_STR(0x008344B8, "G074"));
        CSF_AT(int, CSF_AT(void*, self, 0x08), 0x2AC) = descriptionId;
        return;
    }

    CSF_AT(int, CSF_AT(void*, self, 0x08), 0x298) = 15;
    state = CSF_AT(void*, self, 0x08);
    CSF_AT(uint32_t, state, 0x2A0) = CSF_AT(uint32_t, state, 0x244);
    CSF_AT(uint32_t, state, 0x2A4) = CSF_AT(uint32_t, state, 0x248);

    int descriptionId =
        MenuExtras_ParseDescriptionId(CSF_STR(0x008344B0, "G064"));
    CSF_AT(int, CSF_AT(void*, self, 0x08), 0x2AC) = descriptionId;
}

HOOK(0x004A1F30, HudAlertIcons_EvaluateStageThree, "auto");
