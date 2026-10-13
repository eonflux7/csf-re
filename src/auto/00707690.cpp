#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_fIntroVideoDuration, 0x0082E118);

static int __fastcall C_008633a0_GetRotationMatrix(
    C_008633a0* self, void* /*edx*/, int result)
{
    if (!self->field_001c)
        self->field_001c = reinterpret_cast<std::uint32_t>(RwMatrixCreate());

    const float* quaternion = reinterpret_cast<const float*>(self->field_0010);
    if (quaternion)
    {
        const float scale = g_fIntroVideoDuration /
            (quaternion[0] * quaternion[0] +
             quaternion[1] * quaternion[1] +
             quaternion[2] * quaternion[2] +
             quaternion[3] * quaternion[3]);

        const float scaledY = scale * quaternion[1];
        const float scaledX = scale * quaternion[0];
        const float scaledZ = scale * quaternion[2];
        const float xw = scaledX * quaternion[3];
        const float yw = scaledY * quaternion[3];
        const float zw = scaledZ * quaternion[3];
        const float xx = scaledX * quaternion[0];
        const float yy = scaledY * quaternion[1];
        const float zz = scaledZ * quaternion[2];
        const float yz = scaledZ * quaternion[1];
        const float xz = scaledX * quaternion[2];
        const float xy = scaledY * quaternion[0];

        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x00) =
            g_TransitionOne - (zz + yy);
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x04) = zw + xy;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x08) = xz - yw;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x10) = xy - zw;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x14) =
            g_TransitionOne - (zz + xx);
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x18) = yz + xw;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x20) = xz + yw;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x24) = yz - xw;
        CSF_AT(float, reinterpret_cast<void*>(self->field_001c), 0x28) =
            g_TransitionOne - (yy + xx);

        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_001c), 0x30) = 0;
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_001c), 0x34) = 0;
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_001c), 0x38) = 0;
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_001c), 0x0C) = 3;
    }

    const std::uint32_t matrix = self->field_001c;
    CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x04) = 0;
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(result), 0x00) = matrix;
    return result;
}

HOOK(0x00707690, C_008633a0_GetRotationMatrix, "auto");
