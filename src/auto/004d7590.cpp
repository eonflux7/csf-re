#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (C_008342e8* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* , float a2, char a3));

// Points an entity's task at the direction from its own position toward a target point,
// then starts the C_0082f9f8 task with that heading (radians).
static int __fastcall StartRotateTowardTargetPoint(float** self, void* /*edx*/)
{
    // self+0x08 -> agent; agent+0x14 -> body object (C_008342e8); body+0x30 -> task object
    uint8_t* agent = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    C_008342e8* bodyObj = reinterpret_cast<C_008342e8*>(CSF_AT(uint32_t, agent, 0x14));
    C_0082f9f8* taskObj = reinterpret_cast<C_0082f9f8*>(bodyObj->field_0030);

    float* position = GetObjectPositionVector(bodyObj, nullptr);

    // self+0x0C -> target point; the z component sits at index 17 (+0x44) of the base pointer
    float* targetBase = CSF_AT(float*, self, 0x0C);
    float* targetPos = targetBase + 15;
    float dz = targetBase[17] - position[2];
    float dy = targetPos[1] - position[1];
    float dx = targetPos[0] - position[0];

    alignas(8) uint8_t vectorBytes[0x10];
    float* direction = reinterpret_cast<float*>(vectorBytes);
    direction[0] = dx;
    direction[1] = dy;
    direction[2] = dz;

    double angle = S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(vectorBytes), nullptr);
    float angleRadians = (float)angle;

    return C_0082f9f8_Start(taskObj, nullptr, angleRadians, 0);
}

HOOK(0x004D7590, StartRotateTowardTargetPoint, "auto");
