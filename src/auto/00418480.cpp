#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_SecondaryImpulseScale, 0x0082E1E0);

static char __fastcall C_0082e1e8_ApplyImpulse(
    C_0082e1e8* self, void* /*edx*/, float* position, float* impulse)
{
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* transformSource =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    void* returnedObject = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(self), nullptr));
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, returnedObject, 0)[0x30 / 4])(
                returnedObject, nullptr);

    struct MatrixResult
    {
        float* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
        float referenceHeight;
    };

    MatrixResult matrixResult{};
    matrixResult.referenceHeight = CSF_AT(float, dimensions, 0x4);

    void* transform =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, transformSource, 0)[0x18 / 4])(
                transformSource, nullptr);
    reinterpret_cast<void (__fastcall*)(void*, void*, MatrixResult*)>(
        CSF_AT(void**, transform, 0)[0x8C / 4])(
            transform, nullptr, &matrixResult);

    float* matrix = matrixResult.matrix;
    const float heightOffset =
        matrixResult.referenceHeight * g_SoundActivityThreshold;
    const float centerX = heightOffset * matrix[4] + matrix[12];
    const float centerY = heightOffset * matrix[5] + matrix[13];
    const float centerZ = heightOffset * matrix[6] + matrix[14];

    // The displacement occupies the second half of the normalization buffer.
    float vectorAndDisplacement[6]{};
    vectorAndDisplacement[3] = position[0] - centerX;
    vectorAndDisplacement[4] = position[1] - centerY;
    vectorAndDisplacement[5] = position[2] - centerZ;

    vectorAndDisplacement[0] =
        vectorAndDisplacement[4] * matrix[2] -
        vectorAndDisplacement[5] * matrix[1];
    vectorAndDisplacement[1] =
        vectorAndDisplacement[5] * matrix[0] -
        vectorAndDisplacement[3] * matrix[2];
    vectorAndDisplacement[2] =
        vectorAndDisplacement[3] * matrix[1] -
        vectorAndDisplacement[4] * matrix[0];
    NormalizeVector3(vectorAndDisplacement, nullptr);

    owner = reinterpret_cast<void*>(self->field_0004);
    void* settings = CSF_AT(void*, owner, 0x2C);
    matrix = matrixResult.matrix;

    const float distanceX = position[0] - centerX;
    const float distanceY = position[1] - centerY;
    const float distanceZ = position[2] - centerZ;
    const float distance = std::sqrt(
        distanceZ * distanceZ + distanceY * distanceY +
        distanceX * distanceX);

    self->field_0074 = self->field_0074 -
        (vectorAndDisplacement[0] * impulse[0] +
         vectorAndDisplacement[1] * impulse[1] +
         vectorAndDisplacement[2] * impulse[2]) *
        distance * (CSF_AT(float, settings, 0x1E8) * g_ProgressScale);

    vectorAndDisplacement[0] =
        vectorAndDisplacement[4] * matrix[10] -
        vectorAndDisplacement[5] * matrix[9];
    vectorAndDisplacement[1] =
        vectorAndDisplacement[5] * matrix[8] -
        vectorAndDisplacement[3] * matrix[10];
    vectorAndDisplacement[2] =
        vectorAndDisplacement[3] * matrix[9] -
        vectorAndDisplacement[4] * matrix[8];
    NormalizeVector3(vectorAndDisplacement, nullptr);

    owner = reinterpret_cast<void*>(self->field_0004);
    settings = CSF_AT(void*, owner, 0x2C);
    const char result = static_cast<char>(matrixResult.owned);
    const float secondaryImpulse =
        (vectorAndDisplacement[0] * impulse[0] +
         vectorAndDisplacement[1] * impulse[1] +
         vectorAndDisplacement[2] * impulse[2]) * distance;
    const float secondaryResponse = CSF_AT(float, settings, 0x1EC);
    self->field_0085 = 1;
    self->field_0078 = self->field_0078 -
        secondaryImpulse * (secondaryResponse * g_SecondaryImpulseScale);

    if (result != 0)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrixResult.matrix)));
    return result;
}

HOOK(0x00418480, C_0082e1e8_ApplyImpulse, "auto");
