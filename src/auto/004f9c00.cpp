#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, DispatchConsoleAction, 0x004F9A50,
               (void* self, void* edx, int a2, int a3));

static uint8_t __fastcall C_0083f1f8_DispatchPrimaryActionIfEnabled(
    C_0083f1f8* self, void* /*edx*/, int argument, int /*unused*/)
{
    const uint8_t enabled = self->field_0010;
    if (enabled)
        return static_cast<uint8_t>(
            reinterpret_cast<uintptr_t>(
                DispatchConsoleAction(self, nullptr, 0, argument)));
    return enabled;
}

HOOK(0x004F9C00, C_0083f1f8_DispatchPrimaryActionIfEnabled, "auto");
