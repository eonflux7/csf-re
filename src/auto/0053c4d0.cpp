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

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalState_ResetPending, 0x0050E1E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Object_IsFieldF0NonZero, 0x0050E210, (void* self, void* edx));

// Activates the demo-exit screen: stores the manager, resets the countdown timer,
// refreshes the global value and GUI, and shows the demo-exit notice if no pending menu is set.
// Returns the GUI object pointer.
static uint32_t __fastcall CMenuDemoExit_Activate(CMenuDemoExit* self, void* /*edx*/,
                                                  uint32_t managerArg, uint32_t /*unusedArg*/)
{
    if (self->m_pGui == 0) {
        void* globalState = GetGlobalState_50D190();
        ((void(__fastcall*)(void*, void*, void*))(*reinterpret_cast<void***>(globalState))[6])(
            globalState, nullptr, self);
    }

    self->m_timer = 0.0f;
    self->m_pManager = managerArg;

    void* globalValue = GetGlobalValue_521BB0();
    MenuProgressInput_UpdateGlobalState(globalValue, nullptr);

    void* guiObject = reinterpret_cast<void*>(self->m_pGui);
    ((void(__fastcall*)(void*, void*))(*reinterpret_cast<void***>(guiObject))[0x16c / 4])(
        guiObject, nullptr);

    void* globalState = GetGlobalState_50D190();
    if (Object_IsFieldF0NonZero(globalState, nullptr)) {
        uint32_t pendingMenu = self->m_pPendingMenu;
        if (pendingMenu == 0 || *reinterpret_cast<uint8_t*>(pendingMenu) == 0) {
            void* globalStateForNotice = GetGlobalState_50D190();
            GlobalState_ResetPending(globalStateForNotice, nullptr);
        }
    }

    return self->m_pGui;
}

HOOK(0x0053C4D0, CMenuDemoExit_Activate, "auto");
