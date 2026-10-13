#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// CGuiControl: stores the byte value at +0x1A8 and returns it.
// Original: mov al, [ebp+8]; mov [ecx+0x1A8], al; ret 4 (returns the value in al only).
static char __fastcall CGuiControl_SetRecordByte(CGuiControl* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x1A8) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x44F8C0, CGuiControl_SetRecordByte, "auto");
