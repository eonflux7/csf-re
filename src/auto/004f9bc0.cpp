#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, DevConsole_ApplyState, 0x004F9A50,
                (void* self, void* edx, int a2, int a3));

static uint8_t __fastcall C_0083f1f8_ActivateAlternateIfEnabled(
    C_0083f1f8* self, void* /*edx*/, int /*unused*/)
{
    const uint8_t enabled = self->field_0010;
    if (enabled)
        return static_cast<uint8_t>(
            reinterpret_cast<uintptr_t>(DevConsole_ApplyState(self, nullptr, 1, 1)));
    return enabled;
}

HOOK(0x004F9BC0, C_0083f1f8_ActivateAlternateIfEnabled, "auto");
