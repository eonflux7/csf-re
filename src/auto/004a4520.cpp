#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGameSlot_GetStatus, 0x004E2D60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuState_IsStandbyReady, 0x004929C0, (void* self, void* edx));

// Maps the menu's current state to a result/status code.
static int __fastcall C_00834598_ComputeMenuStateCode(C_00834598* self, void* /*edx*/) {
    C_00833908* menuState = reinterpret_cast<C_00833908*>(self->field_0008);
    CMenuSaveGame* saveGame = reinterpret_cast<CMenuSaveGame*>(CSF_AT(uint32_t, menuState, 0x38));
    int defaultCode = 10;

    if (saveGame && SaveGameSlot_GetStatus(saveGame, nullptr)) {
        if (SaveGameSlot_GetStatus(saveGame, nullptr) == 1)
            return 85;
        if (SaveGameSlot_GetStatus(saveGame, nullptr) == 2)
            return 86;
        return defaultCode;
    }

    if (CSF_AT(uint32_t, menuState, 0x1C4))
        return 74;

    switch (CSF_AT(uint32_t, menuState, 0x1D8)) {
        case 0u:
        case 1u:
            return 10;
        case 2u:
        case 3u:
            return MenuState_IsStandbyReady(menuState, nullptr) ? 61 : 37;
        default:
            return defaultCode;
    }
}

HOOK(0x004A4520, C_00834598_ComputeMenuStateCode, "auto");
