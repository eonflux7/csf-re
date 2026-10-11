#include <cstdint>
#include "hook.h"

// Returns 1 when the two float fields at +0x08 and +0x0C are equal (ordered
// comparison), 0 otherwise, including when either value is NaN.
static int __fastcall AiTaskStates_FieldsEqual(float* self, void* /*edx*/)
{
    float first = self[2];   // +0x08
    float second = self[3];  // +0x0C
    return first == second ? 1 : 0;
}

HOOK(0x41E460, AiTaskStates_FieldsEqual, "auto");
