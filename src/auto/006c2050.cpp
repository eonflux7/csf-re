#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x006C2050
// Reads a pointer at offset 0 of `this`, then loads the 32-bit float stored
// at offset 0xC of the pointed-to object (fld dword -> returned in ST(0)).
static double __fastcall C_Unknown_GetValue(void* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0));
    return CSF_AT(float, inner, 0xC);
}

HOOK(0x006C2050, C_Unknown_GetValue, "auto");

