#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(void, __fastcall, CMenuMpPause_OnActivate, 0x00534AE0, (CMenuMpPause* self, void* edx, int flag));
CSF_FUNC(unsigned int, __fastcall, MenuMpPause_OpenPauseDialog, 0x005353E0, (CMenuMpPause* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());

// Globals referenced by the original.
CSF_GLOBAL(const char*, g_pszMenu_P_OptIng, 0x00899AD4);
CSF_GLOBAL(const char*, g_pszMenu_P_CharMP, 0x00899AFC);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrIn, 0x00899B5C);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFLIn, 0x00899B64);
CSF_GLOBAL(const char*, g_pszMenu_P_MeCaPa, 0x00899B88);
CSF_GLOBAL(uint8_t, g_bUseLbFrInLabel, 0x008B067C);

// Non-zero when the button's stored id is set and equals the event's id (event+4).
static int IsButtonFieldMatchingEvent(uint32_t buttonIdField, void* event)
{
    return buttonIdField != 0 && buttonIdField == CSF_AT(uint32_t, event, 4);
}

// Vtable slot 24 (0x60) on the panel object, called with (menu, 0).
static void PanelBeginLabelUpdate(void* panel, CMenuMpPause* menu)
{
    using PanelMethod = void(__fastcall*)(void* self, void* edx, CMenuMpPause* menu, int flag);
    reinterpret_cast<PanelMethod>(CSF_AT(void**, panel, 0)[24])(panel, nullptr, menu, 0);
}

// Vtable slot 22 (0x58) on the panel object, called with a text pointer.
static void PanelSetLabelText(void* panel, const char* text)
{
    using PanelMethod = void(__fastcall*)(void* self, void* edx, const char* text);
    reinterpret_cast<PanelMethod>(CSF_AT(void**, panel, 0)[22])(panel, nullptr, text);
}

static void __fastcall CMenuMpPause_HandleButtonEvent(CMenuMpPause* self, void* edx, void* event)
{
    if (IsButtonFieldMatchingEvent(self->field_0018, event)) {
        CMenuMpPause_OnActivate(self, nullptr, 1);
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_0028, event)) {
        PanelBeginLabelUpdate(reinterpret_cast<void*>(self->field_0010), self);
        PanelSetLabelText(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_OptIng);
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_001c, event)) {
        PanelBeginLabelUpdate(reinterpret_cast<void*>(self->field_0010), self);
        PanelSetLabelText(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_CharMP);
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_002c, event)) {
        PanelBeginLabelUpdate(reinterpret_cast<void*>(self->field_0010), self);
        if (g_bUseLbFrInLabel != 0) {
            PanelSetLabelText(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_LbFrIn);
        } else {
            PanelSetLabelText(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_LbFLIn);
        }
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_0030, event)) {
        PanelBeginLabelUpdate(reinterpret_cast<void*>(self->field_0010), self);
        PanelSetLabelText(reinterpret_cast<void*>(self->field_0010), g_pszMenu_P_MeCaPa);
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_0020, event)) {
        MenuMpPause_OpenPauseDialog(self, edx);
        return;
    }
    if (IsButtonFieldMatchingEvent(self->field_0024, event)) {
        uint8_t* globalState = reinterpret_cast<uint8_t*>(GetGlobal8B0688());
        CSF_AT(uint8_t, globalState, 0x1401) = 1;
        CSF_AT(uint8_t, globalState, 0x1402) = 0;
    }
}

HOOK(0x00535460, CMenuMpPause_HandleButtonEvent, "auto");
