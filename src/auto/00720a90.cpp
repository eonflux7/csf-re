#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00720A90 (16 bytes): stores a2 into CGuiStatic's dword field at +0x140 and returns it unchanged.
// The field is declared as float (field_0140) in the header, but the original moves the raw dword,
// so the store is performed through CSF_AT(uint32_t) to keep the exact bit pattern (no conversion).
static int __fastcall CGuiStatic_SetQuad(CGuiStatic* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x140) = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x720A90, CGuiStatic_SetQuad, "auto");
