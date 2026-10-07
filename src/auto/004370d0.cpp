#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the dword stored at offset 0x128 of the object.
static int __fastcall CGuiControl_GetField0128(CGuiControl* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0128);
}

HOOK(0x4370D0, CGuiControl_GetField0128, "auto");
