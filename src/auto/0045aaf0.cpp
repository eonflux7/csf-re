#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Writes the Euclidean distance between two 3D points into the output record
// (a1 -> +4 -> float slot) and returns that record pointer.
// a3[0] = object whose +4 holds the position block (x,y,z floats at +4,+8,+12).
// a3[1] = holder whose +4 points to the vector block (x,y,z floats at +4,+8,+12).
static int __stdcall ComputeDistanceToPoint(int a1, int a2, uint32_t* a3)
{
    uint32_t vectorHolderAddr = a3[1];
    uint32_t vectorBlockAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(vectorHolderAddr), 4);
    uint32_t ownerAddr = a3[0];
    uint32_t positionBlockAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(ownerAddr), 4);

    void* vectorBlock = reinterpret_cast<void*>(vectorBlockAddr);
    void* positionBlock = reinterpret_cast<void*>(positionBlockAddr);

    float deltaX = CSF_AT(float, vectorBlock, 4) - CSF_AT(float, positionBlock, 4);
    float deltaY = CSF_AT(float, vectorBlock, 8) - CSF_AT(float, positionBlock, 8);
    float deltaZ = CSF_AT(float, vectorBlock, 12) - CSF_AT(float, positionBlock, 12);

    int result = CSF_AT(int, reinterpret_cast<void*>(a1), 4);
    CSF_AT(float, reinterpret_cast<void*>(result), 4) =
        std::sqrt(deltaZ * deltaZ + deltaX * deltaX + deltaY * deltaY);
    return result;
}

HOOK(0x0045AAF0, ComputeDistanceToPoint, "auto");
