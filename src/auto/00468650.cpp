#include <cstdint>
#include "hook.h"

// Returns 0 when the incoming id/slot equals 12 (the "none" sentinel),
// otherwise passes the value through unchanged.
// The original receives its single argument in ECX (__fastcall), pops no stack bytes.
static int __fastcall MapTwelveToZero(int value, void* /*edx*/)
{
    return value != 12 ? value : 0;
}

HOOK(0x00468650, MapTwelveToZero, "auto");
