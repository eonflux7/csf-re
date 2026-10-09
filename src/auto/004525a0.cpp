#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets the layout-related defaults of a GUI control: sets the 0.5f value at +0x008
// and marks the two sentinel slots at +0x19C and +0x1A0 as -1. Returns -1.
static int __fastcall CGuiControl_ResetLayoutDefaults(CGuiControl* self, void* /*edx*/)
{
    *reinterpret_cast<float*>(&self->field_0008) = 0.5f;
    CSF_AT(int, self, 0x19C) = -1;
    CSF_AT(int, self, 0x1A0) = -1;
    return -1;
}

HOOK(0x4525A0, CGuiControl_ResetLayoutDefaults, "auto");
