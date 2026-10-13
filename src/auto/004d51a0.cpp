#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083dbb0_ResetAutonomousState, 0x004D6EB0, (C_0083dbb0* self, void*));

static int __fastcall C_0083c198_ResetAndInvalidateTargets(C_0083c198* self, void* /*edx*/)
{
    C_0083dbb0_ResetAutonomousState(self, nullptr);
    self->field_03c0 = static_cast<uint32_t>(-1);
    self->field_03c4 = static_cast<uint32_t>(-1);
    return -1;
}

HOOK(0x004D51A0, C_0083c198_ResetAndInvalidateTargets, "auto");
