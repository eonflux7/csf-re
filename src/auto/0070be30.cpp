#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Divides the vector at +0x08 (x, y, z) by the vector at +0x14 (x, y, z)
// component-wise, writes the four floats (x, y, z, 1.0f) to the output
// buffer, and returns that buffer pointer.
static int __fastcall C_0086379c_ComputeRatioVector4(C_0086379c* self, void* /*edx*/, int a2) {
    float numX = *reinterpret_cast<float*>(&self->field_0008);
    float numY = *reinterpret_cast<float*>(&self->field_000c);
    float numZ = CSF_AT(float, self, 0x10);

    float denX = CSF_AT(float, self, 0x14);
    float denY = CSF_AT(float, self, 0x18);
    float denZ = CSF_AT(float, self, 0x1C);

    float resultX = numX / denX;
    float resultY = numY / denY;
    float resultZ = numZ / denZ;

    CSF_AT(float, a2, 0x0) = resultX;
    CSF_AT(float, a2, 0x4) = resultY;
    CSF_AT(float, a2, 0x8) = resultZ;
    CSF_AT(uint32_t, a2, 0xC) = 0x3F800000u; // 1.0f

    return a2;
}

HOOK(0x0070BE30, C_0086379c_ComputeRatioVector4, "auto");
