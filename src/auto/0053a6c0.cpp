#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x50D190, ());

CSF_GLOBAL(const char*, g_pszMenu_P_EEULA, 0x899B94);
CSF_GLOBAL(const char*, g_pszMenu_P_Option, 0x899ACC);
CSF_GLOBAL(const char*, g_pszMenu_P_Credits, 0x899AD0);

// Owner menu (field_0010), vtable slot 24: notifies the owner that this menu's
// button was activated (called with this = owner, child = menu, flag).
static void CMenuMain_NotifyOwnerMenu(CMenuMain* self, int flag)
{
    void* owner = reinterpret_cast<void*>(self->field_0010);
    using OwnerNotifyFn = void(__fastcall*)(void*, void*, CMenuMain*, int);
    reinterpret_cast<OwnerNotifyFn>(CSF_AT(void**, owner, 0)[24])(owner, nullptr, self, flag);
}

// Owner menu (field_0010), vtable slot 22: asks the owner to switch to the named screen.
static int CMenuMain_RequestOwnerScreen(CMenuMain* self, const char* screenName)
{
    void* owner = reinterpret_cast<void*>(self->field_0010);
    using OwnerScreenFn = int(__fastcall*)(void*, void*, const char*);
    return reinterpret_cast<OwnerScreenFn>(CSF_AT(void**, owner, 0)[22])(owner, nullptr, screenName);
}

// Dispatches a button event: eventObj+4 holds the id of the activated control and is
// compared against each button id stored in the menu, in the original order.
static int __fastcall CMenuMain_DispatchButtonEvent(CMenuMain* self, void* /*edx*/, void* eventObj)
{
    using GlobalStateCallFn = int(__fastcall*)(void*, void*);

    // Campanya button (field_001c)
    uint32_t campaignButtonId = self->field_001c;
    if (campaignButtonId != 0 && campaignButtonId == CSF_AT(uint32_t, eventObj, 4)) {
        CMenuMain_NotifyOwnerMenu(self, 0);
        void* globalState = GetGlobalState_50D190();
        return reinterpret_cast<GlobalStateCallFn>(CSF_AT(void**, globalState, 0)[24])(globalState, nullptr);
    }

    // MultiPlayer button (field_0020)
    uint32_t multiplayerButtonId = self->field_0020;
    if (multiplayerButtonId != 0 && multiplayerButtonId == CSF_AT(uint32_t, eventObj, 4)) {
        void* globalState = GetGlobalState_50D190();
        CSF_AT(uint8_t, globalState, 0xD0) = 1;
        CMenuMain_NotifyOwnerMenu(self, 0);
        return CMenuMain_RequestOwnerScreen(self, g_pszMenu_P_EEULA);
    }

    // Opciones button (field_0024)
    uint32_t optionsButtonId = self->field_0024;
    if (optionsButtonId != 0 && optionsButtonId == CSF_AT(uint32_t, eventObj, 4)) {
        CMenuMain_NotifyOwnerMenu(self, 0);
        return CMenuMain_RequestOwnerScreen(self, g_pszMenu_P_Option);
    }

    // Creditos button (field_0028)
    uint32_t creditsButtonId = self->field_0028;
    if (creditsButtonId != 0 && creditsButtonId == CSF_AT(uint32_t, eventObj, 4)) {
        CMenuMain_NotifyOwnerMenu(self, 0);
        return CMenuMain_RequestOwnerScreen(self, g_pszMenu_P_Credits);
    }

    // Fallback button (field_0014): calls this->vtbl[19]() on the menu itself,
    // otherwise returns the stored id.
    uint32_t closeButtonId = self->field_0014;
    if (closeButtonId != 0 && closeButtonId == CSF_AT(uint32_t, eventObj, 4)) {
        using SelfCallFn = int(__fastcall*)(void*, void*);
        return reinterpret_cast<SelfCallFn>(CSF_AT(void**, self, 0)[19])(self, nullptr);
    }
    return static_cast<int>(self->field_0014);
}

HOOK(0x0053A6C0, CMenuMain_DispatchButtonEvent, "auto");
