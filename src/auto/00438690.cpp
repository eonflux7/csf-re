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

// Vtable/address reference carried over from the original's `jmp 0x4382f0` tail jump.
[[maybe_unused]] static void* const kNotificationWidgetVtbl = CSF_VTBL(0x004382F0);

CSF_FUNC(int, __fastcall, NotificationWidget_Start, 0x004382F0, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, NotificationWidget_Hold, 0x00438350, (void* self, void* edx, float a2));

static void __fastcall Hud_UpdateNotificationWidget(S_004382f0* self, void* /*edx*/, float a2)
{
    switch (self->field_0000)
    {
    case 0:
        // Idle state: start the notification (tail call in the original).
        NotificationWidget_Start(self, nullptr, a2);
        return;

    case 1: {
        // Showing: accumulate elapsed time, switch to hold once it reaches the duration.
        float elapsed = a2 + self->field_0010;
        self->field_0010 = elapsed;
        if (elapsed >= self->field_000c)
        {
            self->field_0000 = 2;
            self->field_0010 = 0.0f;
        }
        return;
    }

    case 2:
        // Holding state (tail call in the original).
        NotificationWidget_Hold(self, nullptr, a2);
        return;

    default:
        return;
    }
}

HOOK(0x00438690, Hud_UpdateNotificationWidget, "auto");
