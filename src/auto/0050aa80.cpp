#include <cstdint>
#include "globals.h"
#include "hook.h"

// COptionsFile (module options_file). Stores the two stack arguments into dwords
// +4 and +8 of the object (thiscall: ecx = this) and returns the first argument.
static int __fastcall COptionsFile_SetValuePairReturnFirst(void* self, void* /*edx*/, int firstValue, int secondValue) {
    CSF_AT(int, self, 0x4) = firstValue;
    CSF_AT(int, self, 0x8) = secondValue;
    return firstValue;
}

HOOK(0x0050AA80, COptionsFile_SetValuePairReturnFirst, "auto");
