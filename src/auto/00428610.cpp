#include <cstdint>
#include <cstddef>

#include "hook.h"

// Returns the float stored in record `index` of a 5-float-stride table
// (record i starts at float index 5*i; the value is the record's 5th float, at +0x10 bytes).
static double __fastcall Cheats_GetRecordFloatValue(float* self, void* /*edx*/, int recordIndex) {
    return static_cast<double>(self[5 * recordIndex + 4]);
}

HOOK(0x428610, Cheats_GetRecordFloatValue, "auto");
