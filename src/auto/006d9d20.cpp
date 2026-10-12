#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores the two stack arguments into the object at offsets 0x34 and 0x38
// (thiscall: ecx = this) and returns the first argument.
static int __fastcall StoreTwoArgsAtOffset34ReturnFirst(void* self, void* /*edx*/, int firstValue, int secondValue) {
    CSF_AT(int, self, 0x34) = firstValue;
    CSF_AT(int, self, 0x38) = secondValue;
    return firstValue;
}

HOOK(0x006D9D20, StoreTwoArgsAtOffset34ReturnFirst, "auto");
