#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the pointer stored at this+0x40 and returns the dword at +0x7C
// of that referenced object.
static int __fastcall C_unknown_GetLinkedDword(void* self, void* /*edx*/)
{
    void* linked = CSF_AT(void*, self, 0x40);
    return CSF_AT(int, linked, 0x7c);
}

HOOK(0x476080, C_unknown_GetLinkedDword, "auto");
