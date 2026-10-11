#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee bindings (addresses from the packet).
CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Runs the task's per-frame action only while the active flag at +0x38 is set.
// If no movement has been started yet (field_0010 == 0), starts movement using the
// sub-object at +0x54, then dispatches the action with the caller's argument.
static char __fastcall C_0083dbb0_UpdateWhenActive(C_0083dbb0* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_0038);
    if (result)
    {
        if (!self->field_0010)
        {
            int movementSubObject = static_cast<int>(
                reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x54));
            CharTask_StartMovement(self, nullptr, movementSubObject);
        }
        return Autonomo_DispatchAction(self, nullptr, a2);
    }
    return result;
}

HOOK(0x004D6F40, C_0083dbb0_UpdateWhenActive, "auto");
