#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint64_t, __cdecl, Layout_ApplyIntPair, 0x0042F3B0, (int* a1));
CSF_FUNC(void*, __fastcall, ConstructStringFromText, 0x00434150, (void* self, void* edx, const char* text));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveSceneHandle, 0x00486F50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));

// Refreshes the HUD label widget (at self+0x240) with the text of the current scene's
// menu value, then applies a fixed {270, 530} layout pair and calls the widget's
// update slot. Returns the low byte of the widget's final virtual call.
static char __fastcall HudWidget_RefreshLabelFromSceneValue(void* self, void* /*edx*/) {
    char returnValue = 0;

    if (CSF_AT(void*, self, 0x240) == nullptr)
        return returnValue;
    if (CSF_AT(uint8_t, self, 0x25C) == 0)
        return returnValue;

    C_00862490* sceneMenuContext =
        reinterpret_cast<C_00862490*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (sceneMenuContext == nullptr)
        return returnValue;

    int resolvedHandle =
        ResolveSceneHandle(reinterpret_cast<void*>(sceneMenuContext->field_0044), nullptr);
    if (resolvedHandle == 0)
        return returnValue;

    int valuePointer = Handle_GetValuePointer(reinterpret_cast<void*>(resolvedHandle), nullptr);
    if (valuePointer != 0) {
        uint8_t* valueBase = reinterpret_cast<uint8_t*>(valuePointer);
        const char* valueText;
        if (CSF_AT(uint32_t, valueBase, 0x38) < 0x10u)
            valueText = reinterpret_cast<const char*>(valueBase + 0x24);
        else
            valueText = CSF_AT(const char*, valueBase, 0x24);

        // sized by the original's frame (0x24 bytes): struct sizes in auto_classes.h are lower bounds
        alignas(8) uint8_t textStorageBytes[0x24];
        C_0082d410& textStorage = *reinterpret_cast<C_0082d410*>(textStorageBytes);
        ConstructStringFromText(&textStorage, nullptr, valueText);

        uint32_t textData = textStorage.field_0004;
        if (CSF_AT(uint32_t, &textStorage, 0x18) < 0x10u)
            textData = reinterpret_cast<uint32_t>(&textStorage.field_0004);
        int convertedText = Text_ConvertToDisplayEncoding(static_cast<int>(textData), 8);

        // Widget slot 0x40 (16): set text.
        void* widgetForText = CSF_AT(void*, self, 0x240);
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, widgetForText, 0)[16]))(
            widgetForText, nullptr, convertedText);

        CMenuSaveGame_ResetStringStorage(&textStorage, nullptr);
    }

    int layoutPair[2] = {270, 530};
    Layout_ApplyIntPair(layoutPair);

    // Widget slot 0x50 (20): apply layout pair.
    void* widgetForLayout = CSF_AT(void*, self, 0x240);
    ((void(__fastcall*)(void*, void*, int*))(CSF_AT(void**, widgetForLayout, 0)[20]))(
        widgetForLayout, nullptr, layoutPair);

    // Widget slot 0x08 (2): update; its result's low byte is the return value.
    void* widgetForUpdate = CSF_AT(void*, self, 0x240);
    int updateResult =
        ((int(__fastcall*)(void*, void*))(CSF_AT(void**, widgetForUpdate, 0)[2]))(
            widgetForUpdate, nullptr);
    returnValue = static_cast<char>(updateResult);
    return returnValue;
}

HOOK(0x004345E0, HudWidget_RefreshLabelFromSceneValue, "auto");
