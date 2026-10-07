#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Constant data referenced by the accessor (a 4-byte float slot in .rdata/.data).
CSF_GLOBAL(float, g_Value_0082D4CC, 0x82D4CC);

// 0x0070DDF0 (7 bytes): loads the float at 0x82D4CC onto the x87 stack and returns it.
// The x87 result is handed back as a double by the caller ABI; the value itself is the
// unmodified 32-bit float, so returning a float promoted to double is bit-exact.
static double __fastcall C_0083e500_GetValue(C_0083e500* self, void* /*edx*/)
{
    (void)self; // `this` is never touched by the original (ecx is ignored).
    return g_Value_0082D4CC;
}

HOOK(0x70DDF0, C_0083e500_GetValue, "auto");
