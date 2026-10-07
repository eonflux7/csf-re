#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the 32-bit id/handle stored at this+4 and reports whether it is not -1
// (i.e. whether the object currently has a valid/assigned id).
static int __fastcall HasValidId(void* self, void* /*edx*/)
{
    return CSF_AT(int32_t, self, 0x4) != -1;
}

HOOK(0x4AD7F0, HasValidId, "auto");
