#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00860F00);

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));

static void __fastcall CCamera_RotateAroundPoint(
    CCamera* self, void* /*edx*/, float* pivot,
    float firstAngle, float secondAngle)
{
    if (firstAngle == g_fZero && secondAngle == g_fZero)
        return;

    void* transform = reinterpret_cast<std::uint8_t*>(self) + 0x10;
    void** translationVtable = reinterpret_cast<void**>(self->field_0010);

    alignas(8) std::uint8_t translationBytes[0xC];
    CSF_AT(float, translationBytes, 0x0) = -pivot[0];
    CSF_AT(float, translationBytes, 0x4) = -pivot[1];
    CSF_AT(float, translationBytes, 0x8) = -pivot[2];

    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        translationVtable[0x20 / 4])(
            transform, nullptr, reinterpret_cast<float*>(translationBytes));

    if (firstAngle != g_fZero)
    {
        S_00526d40** axisHandle =
            reinterpret_cast<S_00526d40** (__fastcall*)(void*, void*, void*)>(
                reinterpret_cast<void**>(self->field_0010)[0x8C / 4])(
                    transform, nullptr, translationBytes + 0x4);

        S_00526d40* axis = *axisHandle;
        void** rotationVtable = reinterpret_cast<void**>(self->field_0010);

        alignas(8) std::uint8_t quaternionBytes[0x10];
        float* quaternion = GfxMatrix_SetQuaternionFromAxisAngle(
            reinterpret_cast<float*>(quaternionBytes), nullptr,
            axis, firstAngle * g_DegreesToRadians);

        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            rotationVtable[0x60 / 4])(transform, nullptr, quaternion);

        if (CSF_AT(std::uint8_t, translationBytes, 0x8))
            RwMatrixDestroy(CSF_AT(std::uint32_t*, translationBytes, 0x4));
    }

    if (secondAngle != g_fZero)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
            reinterpret_cast<void**>(self->field_0010)[0x40 / 4])(
                transform, nullptr, secondAngle);
    }

    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        reinterpret_cast<void**>(self->field_0010)[0x20 / 4])(
            transform, nullptr, pivot);
}

HOOK(0x006D2DB0, CCamera_RotateAroundPoint, "auto");
