#include "hook.h"
#include "types/auto_classes.h"

static float* __fastcall S_006abb80_WriteBoxCorners(
    S_006abb80* self, void* /*edx*/, float* corners)
{
    const float axisXY = self->field_000c * self->field_001c;
    const float axisXX = self->field_000c * self->field_0018;
    const float axisXZ = self->field_000c * self->field_0020;

    const float extentY = self->field_0010;
    const float axisYX = extentY * self->field_0024;
    const float axisYY = extentY * self->field_0028;
    const float axisYZ = extentY * self->field_002c;

    const float extentZ = self->field_0014;
    const float axisZX = extentZ * self->field_0030;
    const float axisZY = extentZ * self->field_0034;
    const float axisZZ = extentZ * self->field_0038;

    // Read the center anew for each corner, allowing output to alias self.
    float z = self->field_0008 - axisXZ;
    float x = self->field_0000 - axisXX - axisYX;
    float y = self->field_0004 - axisXY - axisYY;
    corners[0] = x - axisZX;
    corners[1] = y - axisZY;
    corners[2] = z - axisYZ - axisZZ;

    z = axisXZ + self->field_0008;
    x = axisXX + self->field_0000 - axisYX;
    y = axisXY + self->field_0004 - axisYY;
    corners[3] = x - axisZX;
    corners[4] = y - axisZY;
    corners[5] = z - axisYZ - axisZZ;

    z = axisXZ + self->field_0008;
    x = axisXX + self->field_0000 + axisYX;
    y = axisXY + self->field_0004 + axisYY;
    corners[6] = x - axisZX;
    corners[7] = y - axisZY;
    corners[8] = z + axisYZ - axisZZ;

    z = self->field_0008 - axisXZ;
    x = self->field_0000 - axisXX + axisYX;
    y = self->field_0004 - axisXY + axisYY;
    corners[9] = x - axisZX;
    corners[10] = y - axisZY;
    corners[11] = z + axisYZ - axisZZ;

    z = self->field_0008 - axisXZ;
    x = self->field_0000 - axisXX - axisYX;
    y = self->field_0004 - axisXY - axisYY + axisZY;
    z = z - axisYZ + axisZZ;
    corners[12] = x + axisZX;
    corners[13] = y;
    corners[14] = z;

    z = axisXZ + self->field_0008;
    x = axisXX + self->field_0000 - axisYX;
    y = axisXY + self->field_0004 - axisYY;
    corners[15] = x + axisZX;
    corners[16] = y + axisZY;
    corners[17] = z - axisYZ + axisZZ;

    z = axisXZ + self->field_0008;
    x = axisXX + self->field_0000 + axisYX;
    y = axisXY + self->field_0004 + axisYY;
    corners[18] = x + axisZX;
    corners[19] = y + axisZY;
    corners[20] = z + axisYZ + axisZZ;

    z = self->field_0008 - axisXZ;
    x = self->field_0000 - axisXX + axisYX;
    y = self->field_0004 - axisXY + axisYY + axisZY;
    corners[21] = x + axisZX;
    z = z + axisYZ + axisZZ;
    corners[22] = y;
    corners[23] = z;

    return corners + 21;
}

HOOK(0x006ABC40, S_006abb80_WriteBoxCorners, "auto");
