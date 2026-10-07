#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant tag/name string "TDudoso" (constant data at 0x0083A35C).
// The function ignores both `this` and any arguments.
static const char* __fastcall GetName_0083a2d8(void* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x0083A35C, "TDudoso");
}

HOOK(0x004B25F0, GetName_0083a2d8, "auto");
