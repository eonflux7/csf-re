#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees bound by address.
CSF_FUNC(int, __fastcall, CMenuMpSessions_ApplySessionControlStates, 0x00560500, (CMenuMpSessions* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpSessions_RefreshSessionDisplay, 0x00560890, (CMenuMpSessions* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (void* self, void* edx));

// Pending session mode published by another screen; consumed (reset to -1) here.
CSF_GLOBAL(uint32_t, g_pendingSessionMode, 0x0089A5F8);

// Looks up a child control by name through the container object at field_0040
// (vtable slot 63, +0xFC). The container is re-read on every call, as the original does.
static void* CMenuMpSessions_FindChildControl(CMenuMpSessions* self, const char* controlName) {
    void* controlContainer = reinterpret_cast<void*>(self->field_0040);
    return ((void*(__fastcall*)(void*, void*, const char*))CSF_AT(void**, controlContainer, 0)[63])(
        controlContainer, nullptr, controlName);
}

// Posts a state tag to the status control at field_0074 (vtable slot 169, +0x2A4).
static void CMenuMpSessions_SetStatusTag(CMenuMpSessions* self, uint32_t stateTag) {
    void* statusControl = reinterpret_cast<void*>(self->field_0074);
    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, statusControl, 0)[169])(
        statusControl, nullptr, stateTag);
}

static int __fastcall CMenuMpSessions_EnterScreen(CMenuMpSessions* self, void* /*edx*/, int unusedA2, int unusedA3) {
    (void)unusedA2;
    (void)unusedA3;

    // Cache the edit controls; clear their flag bytes when they exist.
    void* editNombreSesion = CMenuMpSessions_FindChildControl(self, CSF_STR(0x0084592C, "EdNombreSesion"));
    self->field_003c = reinterpret_cast<uint32_t>(editNombreSesion);
    if (editNombreSesion) {
        CSF_AT(uint8_t, editNombreSesion, 0x1C7) = 0;
    }

    void* editPassword = CMenuMpSessions_FindChildControl(self, CSF_STR(0x00845920, "EdPassword"));
    self->field_01fc = reinterpret_cast<uint32_t>(editPassword);
    if (editPassword) {
        CSF_AT(uint8_t, editPassword, 0x1C6) = 0;
    }

    // Cache the buttons.
    void* butPartidaPrivada = CMenuMpSessions_FindChildControl(self, CSF_STR(0x0084590C, "ButPartidaPrivada"));
    self->field_0208 = reinterpret_cast<uint32_t>(butPartidaPrivada);
    self->field_00a4 = reinterpret_cast<uint32_t>(CMenuMpSessions_FindChildControl(self, CSF_STR(0x00845904, "ButSave")));
    self->field_0038 = reinterpret_cast<uint32_t>(CMenuMpSessions_FindChildControl(self, CSF_STR(0x008458F8, "ButEmpezar")));

    CMenuMpSessions_ApplySessionControlStates(self, nullptr);

    // Reset the session listbox (vtable slot 170, +0x2A8) with argument 0.
    void* listboxPartidas = reinterpret_cast<void*>(self->field_00ac);
    if (listboxPartidas) {
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, listboxPartidas, 0)[170])(
            listboxPartidas, nullptr, 0u);
    }

    // Consume the pending session mode, if one was published.
    self->field_0018 = 0;
    const uint32_t pendingMode = g_pendingSessionMode;
    if (pendingMode != 0xFFFFFFFFu) {
        self->field_0018 = pendingMode;
        g_pendingSessionMode = 0xFFFFFFFFu;
    }

    // Post the status tag matching the session mode.
    const uint32_t sessionMode = self->field_0018;
    if (sessionMode == 0) {
        CMenuMpSessions_SetStatusTag(self, 0x636d3031u);
    } else if (sessionMode == 1) {
        CMenuMpSessions_SetStatusTag(self, 0x636d3032u);
    } else if (sessionMode == 3) {
        CMenuMpSessions_SetStatusTag(self, 0x636d3034u);
    }

    CMenuMpSessions_RefreshSessionDisplay(self, nullptr);

    self->field_01e4 = 0;
    self->field_001c = 2;

    // Virtual slot 12 (+0x30). Only the low byte (al) is the flag; the full eax is kept as the result.
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[12])(self, nullptr);

    void* menuPanelControl = reinterpret_cast<void*>(self->field_007c);
    if ((result & 0xFF) != 0) {
        if (menuPanelControl) {
            // Virtual slot 26 (+0x68) with arguments (0, 1, 1).
            result = ((int(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, menuPanelControl, 0)[26])(
                menuPanelControl, nullptr, 0, 1, 1);
        }
        void* sliderControl = reinterpret_cast<void*>(self->field_0034);
        if (sliderControl) {
            return CGuiSlider_ForwardSelfToParent(sliderControl, nullptr);
        }
    } else {
        if (menuPanelControl) {
            // Virtual slot 26 (+0x68) with arguments (1, 1, 1).
            ((void(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, menuPanelControl, 0)[26])(
                menuPanelControl, nullptr, 1, 1, 1);
        }
        // Virtual slot 24 (+0x60), result discarded.
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[24])(self, nullptr);
        // Virtual slot 23 (+0x5C), its eax is the return value.
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[23])(self, nullptr);
    }
    return result;
}

HOOK(0x00561650, CMenuMpSessions_EnterScreen, "auto");
