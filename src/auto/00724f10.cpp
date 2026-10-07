#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00724F10 (7 bytes, ret 0): reads a dword field at offset 0x1A0 of the object.
// The offset is not part of CGuiButton's declared fields (they start at 0x1C8),
// so it lives somewhere in the undeclared base layout and is accessed via CSF_AT.
static uint32_t __fastcall CGuiButton_GetValue_01a0(CGuiButton* self, void* /*edx*/)
{
    return CSF_AT(uint32_t, self, 0x1a0);
}

HOOK(0x00724F10, CGuiButton_GetValue_01a0, "auto");
