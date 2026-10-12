#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, Vec3_CrossProductInto, 0x00402060, (float* self, void* , float* out, float* other));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(uint32_t*, __fastcall, S_00526d40_ConvertDirection, 0x006ABA10, (S_00526d40* self, void* , uint32_t* output));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionAxisAngle, 0x006AE970, (float* self, void* , S_00526d40* axis, float angle));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_RotationPi, 0x0082E07C);
CSF_GLOBAL(float, g_RotationTenPi, 0x0082E084);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static void __fastcall C_0082e090_ApplyRotationInput(
    C_0082e090* self, void* /*edx*/, float amount, S_00526d40* direction)
{
    if (amount == g_fZero)
        return;

    if (*reinterpret_cast<float*>(&direction->field_0000) == g_ZeroVectorX
        && CSF_AT(float, direction, 0x4) == g_ZeroVectorY
        && CSF_AT(float, direction, 0x8) == g_ZeroVectorZ)
        return;

    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* object =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* transform =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);

    alignas(8) uint8_t quaternionBytes[0x10];
    alignas(8) uint8_t convertedDirectionBytes[0xC];
    alignas(8) uint8_t crossProductBytes[0xC];
    alignas(8) uint8_t matrixResultBytes[0x8];

    float* quaternion = reinterpret_cast<float*>(quaternionBytes);

    switch (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50))
    {
    case 8:
    case 9:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x14:
    case 0x15:
    {
        const float angle = amount * g_RotationPi;
        float* axis = Vec3_CrossProductInto(
            reinterpret_cast<float*>(&self->field_009c), nullptr,
            reinterpret_cast<float*>(crossProductBytes),
            reinterpret_cast<float*>(direction));

        GfxMatrix_SetQuaternionFromAxisAngle(
            quaternion, nullptr, reinterpret_cast<S_00526d40*>(axis), angle);

        ((void (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, transform, 0)[0x60 / 4])(
                transform, nullptr, quaternion);
        break;
    }

    case 0xA:
    {
        void** transformVtable = CSF_AT(void**, transform, 0);
        uint32_t* converted = S_00526d40_ConvertDirection(
            direction, nullptr,
            reinterpret_cast<uint32_t*>(convertedDirectionBytes));

        ((void (__fastcall*)(void*, void*, uint32_t*))
            transformVtable[0x58 / 4])(transform, nullptr, converted);
        break;
    }

    case 0xB:
    {
        int childIndex =
            ((int (__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x98 / 4])(
                    object, nullptr, 2200);
        if (childIndex == -1)
            break;

        void* child =
            ((void* (__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x8C / 4])(
                    object, nullptr, childIndex);
        void* childTransform =
            ((void* (__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr);

        ((void (__fastcall*)(void*, void*, void*))
            CSF_AT(void**, childTransform, 0)[0x8C / 4])(
                childTransform, nullptr, matrixResultBytes);

        GfxMatrix_InitializeQuaternion(
            reinterpret_cast<uint32_t*>(quaternionBytes), nullptr);

        const float angle = amount * g_RotationTenPi;
        auto* axis = reinterpret_cast<S_00526d40*>(
            reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t*, matrixResultBytes, 0)) + 0x10);
        GfxMatrix_SetQuaternionAxisAngle(quaternion, nullptr, axis, angle);

        ((void (__fastcall*)(void*, void*, float*))
            CSF_AT(void**, transform, 0)[0x60 / 4])(
                transform, nullptr, quaternion);

        if (CSF_AT(uint8_t, matrixResultBytes, 0x4))
            RwMatrixDestroy(CSF_AT(uint32_t*, matrixResultBytes, 0));
        break;
    }

    default:
        return;
    }
}

HOOK(0x00415E70, C_0082e090_ApplyRotationInput, "auto");
