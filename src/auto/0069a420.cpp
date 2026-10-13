#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Mode 0 and any unlisted mode use a2 as the new value, mode 1 adds field_0008,
// mode 2 adds field_000c. The value is stored in field_0008 only if it is in [0, field_000c].
static char __fastcall C_0085e7b8_SetOffsetFromMode(C_0085e7b8* self, void* /*edx*/, int a2, int a3) {
    int candidate;
    if (a3 == 1) {
        candidate = a2 + static_cast<int>(self->field_0008);
    } else if (a3 == 2) {
        candidate = static_cast<int>(self->field_000c) + a2;
    } else {
        candidate = a2;
    }

    if (candidate < 0 || candidate > static_cast<int>(self->field_000c)) {
        return 0;
    }
    self->field_0008 = static_cast<uint32_t>(candidate);
    return 1;
}

HOOK(0x69A420, C_0085e7b8_SetOffsetFromMode, "auto");
