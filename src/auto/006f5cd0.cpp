#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, RwCameraSetNearClipPlane, 0x00628E90, (int a1, float a2));
CSF_FUNC(int, __cdecl, RwCameraSetFarClipPlane, 0x00628EC0, (int a1, float a2));
CSF_FUNC(int, __cdecl, RwCameraSetProjection, 0x00628F90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraSetViewWindow, 0x00629000, (int a1, uint32_t* a2));
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, GfxMatrix_OrientAlongVector, 0x006ADFA0, (S_006ada10* self, void* , void* sourceData));

static char __fastcall S_006e48a0_UpdateOrthographicCamera(
    S_006e48a0* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_003c);
    if (!result)
        return result;

    alignas(8) std::uint8_t viewWindowBytes[0x8];
    alignas(8) std::uint8_t matrixBytes[0x10];
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

    void** methods = reinterpret_cast<void**>(self->field_0000);
    *reinterpret_cast<std::uint8_t*>(&self->field_0034) = 1;

    const std::uint32_t* translation =
        reinterpret_cast<const std::uint32_t* (__fastcall*)(void*, void*)>(
            methods[0x24 / 4])(self, nullptr);

    void* direction =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x28 / 4])(
                self, nullptr);

    CSF_AT(float, matrixBytes, 0xC) = static_cast<float>(
        reinterpret_cast<double (__fastcall*)(void*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x8 / 4])(
                self, nullptr));

    std::memcpy(matrixBytes + 0x8, &self->field_0010, sizeof(float));
    RwCameraSetProjection(static_cast<int>(self->field_0030), 2);

    CSF_AT(float, viewWindowBytes, 0x0) = CSF_AT(float, matrixBytes, 0xC);
    CSF_AT(float, viewWindowBytes, 0x4) = CSF_AT(float, matrixBytes, 0xC);
    RwCameraSetViewWindow(
        static_cast<int>(self->field_0030),
        reinterpret_cast<std::uint32_t*>(viewWindowBytes));

    RwCameraSetFarClipPlane(
        static_cast<int>(self->field_0030),
        CSF_AT(float, matrixBytes, 0x8));
    RwCameraSetNearClipPlane(static_cast<int>(self->field_0030), 10.0f);

    matrix.field_0000 =
        CSF_AT(std::uint32_t,
               reinterpret_cast<void*>(self->field_0030), 0x4) + 0x10;
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 0;

    GfxMatrix_OrientAlongVector(&matrix, nullptr, direction);
    GfxMatrix_CopyTranslationAndUpdate(&matrix, nullptr, translation);

    result = static_cast<char>(CSF_AT(std::uint8_t, matrixBytes, 0x4));
    if (result)
        return static_cast<char>(RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(matrix.field_0000)));

    return result;
}

HOOK(0x006F5CD0, S_006e48a0_UpdateOrthographicCamera, "auto");
