#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));

// Returns base handle (field +0x08) plus 80 bytes per slot index.
static int __fastcall C_00840af8_ComputeHandleFromBaseAndSlot(C_00840af8* self, void* /*edx*/)
{
    int slotIndex = ComputeChildHandleFromSlot(self, nullptr);
    return static_cast<int>(self->field_0008) + 80 * slotIndex;
}

HOOK(0x515FA0, C_00840af8_ComputeHandleFromBaseAndSlot, "auto");
