#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x00528840: performs the actual update for this menu message box (tail-jump target).
CSF_FUNC(void, __fastcall, MenuMessageBox_UpdateIndicators, 0x528840, (void* self, void* edx, float a2));

// Re-invokes the update only when both the "active" byte (+5) and the "enabled" byte (+4) are set.
static void __fastcall CMenuMessageBox_UpdateIfEnabled(void* self, void* /*edx*/, float a2)
{
    if (CSF_AT(uint8_t, self, 5) != 0)
    {
        if (CSF_AT(uint8_t, self, 4) != 0)
            MenuMessageBox_UpdateIndicators(self, nullptr, a2);
    }
}

HOOK(0x5288D0, CMenuMessageBox_UpdateIfEnabled, "auto");
