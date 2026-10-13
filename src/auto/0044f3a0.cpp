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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));

static bool __fastcall CGuiControl_SetSelectionEnabled(CGuiControl* self, void* /*edx*/, char enabled)
{
    // The result is the flag "enabled == 0" as tested after the optional reset call.
    bool result = (enabled == 0);
    if (!enabled) {
        int globalState = GetGlobalState_50D190();
        C_00830578* menuController = CSF_AT(C_00830578*, reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), 0x18);
        C_00830578_ResetMenuController(menuController, nullptr);
    }
    *reinterpret_cast<uint8_t*>(&self->m_pSizeAnim) = result ? 1 : 0;
    return result;
}

HOOK(0x0044F3A0, CGuiControl_SetSelectionEnabled, "auto");
