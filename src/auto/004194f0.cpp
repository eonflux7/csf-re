#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));

static int __fastcall C_0082e1e8_ApplyAxisRotations(
    C_0082e1e8* self, void* /*edx*/, C_0082e1e8* rotations)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* controller =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
        (owner, nullptr);

    void* transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[0x18 / 4])
        (controller, nullptr);

    const float* initialPosition =
        ((float* (__fastcall*)(void*, void*))CSF_AT(void**, transform, 0)[0x10 / 4])
        (transform, nullptr);

    float position[3] = {
        initialPosition[0], initialPosition[1], initialPosition[2]
    };

    struct MatrixOutput {
        std::uint32_t* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
        std::uint32_t iteration;
    };
    static_assert(sizeof(MatrixOutput) == 0xC);

    MatrixOutput matrixOutput{};
    if (rotations->field_0054 != 0) {
        std::uint32_t recordOffset = 0;
        do {
            float* record = reinterpret_cast<float*>(
                reinterpret_cast<std::uint8_t*>(rotations->field_004c) + recordOffset);

            // The axis occupies the three floats immediately after the quaternion.
            float quaternionAndAxis[7]{};
            quaternionAndAxis[4] = record[3] - record[0];
            quaternionAndAxis[5] = record[4] - record[1];
            quaternionAndAxis[6] = record[5] - record[2];

            GfxMatrix_SetQuaternionFromAxisAngle(
                quaternionAndAxis, nullptr,
                reinterpret_cast<S_00526d40*>(quaternionAndAxis + 4), record[6]);
            GfxMatrix_CreateQuaternionMatrix(
                quaternionAndAxis, nullptr, reinterpret_cast<int>(&matrixOutput));

            transform =
                ((void* (__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[0x18 / 4])
                (controller, nullptr);
            ((void (__fastcall*)(void*, void*, float*))CSF_AT(void**, transform, 0)[0x60 / 4])
                (transform, nullptr, quaternionAndAxis);

            position[0] -= record[3];
            position[1] -= record[4];
            position[2] -= record[5];

            float rotatedPosition[3] = {0.0f, 0.0f, 0.0f};
            RwV3dTransformVectors(
                reinterpret_cast<int>(rotatedPosition),
                reinterpret_cast<int>(position), 1,
                reinterpret_cast<int>(matrixOutput.matrix));

            position[0] = rotatedPosition[0] + record[3];
            position[1] = rotatedPosition[1] + record[4];
            position[2] = rotatedPosition[2] + record[5];

            if (matrixOutput.owned)
                RwMatrixDestroy(matrixOutput.matrix);

            ++matrixOutput.iteration;
            recordOffset += 28;
        } while (matrixOutput.iteration < rotations->field_0054);
    }

    transform =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[0x18 / 4])
        (controller, nullptr);
    return
        ((int (__fastcall*)(void*, void*, float*))CSF_AT(void**, transform, 0)[0x14 / 4])
        (transform, nullptr, position);
}

HOOK(0x004194F0, C_0082e1e8_ApplyAxisRotations, "auto");
