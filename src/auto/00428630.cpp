#include <cstdint>
#include <cstddef>

#include "hook.h"

// Returns the float stored in record `recordIndex` of a 5-float-stride table
// (record i starts at float index 5*i; the value is the float at index 5*i+5, i.e. +0x14 bytes into the record).
static double __fastcall Cheats_GetRecordSixthFloatValue(float* self, void* /*edx*/, int recordIndex) {
    return static_cast<double>(self[5 * recordIndex + 5]);
}

HOOK(0x428630, Cheats_GetRecordSixthFloatValue, "auto");
