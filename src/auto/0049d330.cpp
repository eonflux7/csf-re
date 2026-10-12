#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: GetObjectPositionVector (0x48E430). Forwarder; called with ecx = this and no pushed args.
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x48E430, (C_008342e8* self, void* edx));

// Copies the three-dword position vector of this object into outVector and returns outVector.
static uint32_t* __fastcall C_00834050_CopyObjectPosition(C_00834050* self, void* /*edx*/, uint32_t* outVector, int /*unused*/)
{
    uint32_t* positionVector = GetObjectPositionVector(reinterpret_cast<C_008342e8*>(self), nullptr);
    outVector[0] = positionVector[0];
    outVector[1] = positionVector[1];
    outVector[2] = positionVector[2];
    return outVector;
}

HOOK(0x0049D330, C_00834050_CopyObjectPosition, "auto");
