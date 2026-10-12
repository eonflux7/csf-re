#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, C_0083e9d0_TestPairWithSelfVector, 0x004E5C30, (C_0083e9d0* self, void* , float* a2, float* a3));

// Copies the 3-float position at arg0 into a local vector and asks the
// pathing object (this->owner(+0x2C)->field(+0x3C)) whether that pair can be reached.
static bool __fastcall AiCore_CanReachPosition(void* self, void* /*edx*/, float* arg0, float* a3)
{
    uint8_t* ownerObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x2C));
    C_0083e9d0* pathObject = CSF_AT(C_0083e9d0*, ownerObject, 0x3C);

    float a2[3];
    a2[0] = arg0[0];
    a2[1] = arg0[1];
    a2[2] = arg0[2];

    return C_0083e9d0_TestPairWithSelfVector(pathObject, nullptr, a2, a3);
}

HOOK(0x004AA390, AiCore_CanReachPosition, "auto");
