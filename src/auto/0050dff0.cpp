#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Calls virtual slot 0x3C (index 15) of a screen object with no arguments.
// The original invokes this on every screen the manager owns before tail-jumping.
static void CMenuManager_CallScreenSlot3C(uint32_t screenPtr) {
    void* screen = reinterpret_cast<void*>(screenPtr);
    auto closeFn = reinterpret_cast<void(__fastcall*)(void*, void*)>(
        (*reinterpret_cast<void***>(screen))[0x3C / 4]);
    closeFn(screen, nullptr);
}

// Closes every screen owned by the menu manager, then tail-jumps into the
// virtual slot 0x1C of the last screen (field_007c) with the caller's
// stack arguments and return address left untouched.
static hook::Tail __fastcall CMenuManager_CloseAllScreens(void* self, const uint32_t* args) {
    (void)args;
    auto* menu = static_cast<CMenuManager*>(self);

    CMenuManager_CallScreenSlot3C(menu->m_pScreenIntro);
    CMenuManager_CallScreenSlot3C(menu->m_pScreenGame);
    CMenuManager_CallScreenSlot3C(menu->m_pScreenGameMenu);
    CMenuManager_CallScreenSlot3C(menu->field_0050);
    CMenuManager_CallScreenSlot3C(menu->field_0044);
    CMenuManager_CallScreenSlot3C(menu->m_pScreenGameOptionsInGame);
    CMenuManager_CallScreenSlot3C(menu->m_pScreenTactic);
    CMenuManager_CallScreenSlot3C(menu->field_0034);
    CMenuManager_CallScreenSlot3C(menu->field_0058);
    CMenuManager_CallScreenSlot3C(menu->field_0060);
    CMenuManager_CallScreenSlot3C(menu->m_pScreenMsgBox);

    void* last = reinterpret_cast<void*>(menu->field_007c);
    return hook::tail_jump(last, (*reinterpret_cast<void***>(last))[0x1C / 4]);
}

HOOK_TAIL(0x0050DFF0, CMenuManager_CloseAllScreens, "game_bdd_loader");
