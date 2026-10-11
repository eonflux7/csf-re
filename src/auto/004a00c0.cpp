#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Releases the hud_alert_icons subsystem state owned by this object.
CSF_FUNC(int, __fastcall, HudAlertIcons_ReleaseSubsystem, 0x0049FFC0, (void* self, void* edx));

// If the active flag is set, releases the subsystem and clears the flag.
// Returns the flag value read on entry, or the release routine's result (low byte) when the flag was set.
static char __fastcall Scene_ResetSubsystemAC(C_008343a4* self, void* /*edx*/) {
    char result = static_cast<char>(self->field_0004);
    if (result) {
        result = static_cast<char>(HudAlertIcons_ReleaseSubsystem(self, nullptr));
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x4A00C0, Scene_ResetSubsystemAC, "auto");
