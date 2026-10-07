#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00725120 (16 bytes): stores the 32-bit argument at self+0x148 and returns it unchanged.
static int __fastcall CGuiControl_SetField0148(CGuiControl* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x148) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x725120, CGuiControl_SetField0148, "auto");
