#include <cstdint>
#include "hook.h"

// Copies one dword from *src to *dest when dest is non-null and returns dest.
// __stdcall: callee pops 8 bytes (two pointer arguments).
static uint32_t* __stdcall CopyDwordIfNotNull(uint32_t* dest, const uint32_t* src)
{
    uint32_t* result = dest;
    if (dest)
        *dest = *src;
    return result;
}

HOOK(0x00730830, CopyDwordIfNotNull, "auto");
