#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));
CSF_FUNC(unsigned int, __cdecl, sub_42DE60, 0x0042DE60, (float* a1));
CSF_FUNC(int, __fastcall, sub_42EE00, 0x0042EE00, (void* self, void* edx, float a2, uint32_t* a3, unsigned int* a4));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(double, __fastcall, ComputeScalarForResolvedObject, 0x0048F220, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, ScaleMatrix, 0x006ADB60, (void* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
CSF_FUNC(int, __fastcall, TranslateMatrixHandle, 0x006ADBB0, (void* self, void* edx, float x, float y, float z));
CSF_FUNC(int, __fastcall, RotateMatrixHandle, 0x006AE210, (void* self, void* edx, float radians));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_HudDegreesToRadians, 0x00830670);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint32_t Hud_FloatArgumentBits(float value)
{
    std::uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static C_008342e8* __fastcall S_0042ee00_UpdateOrientationWidget(
    S_0042ee00* self, void* /*edx*/, const void* orientation)
{
    const float inputAngle = CSF_AT(float, orientation, 0);
    auto* context = reinterpret_cast<C_008342e8*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!context)
        return context;

    const double sceneAngle = ComputeScalarForResolvedObject(context, nullptr);
    const float halfWidth = static_cast<float>(g_ScreenWidth) * g_HalfDurationScale;
    const float halfHeight = static_cast<float>(g_ScreenHeight) * g_HalfDurationScale;

    std::uint32_t rotationBits = 0;
    unsigned int position[2] = {0, 0};
    const float radians = static_cast<float>(sceneAngle * g_HudDegreesToRadians);
    const float combinedAngle = static_cast<float>(
        MenuMpCharacterSelect_NormalizeAngle(radians) + inputAngle);
    const float normalizedAngle = static_cast<float>(
        MenuMpCharacterSelect_NormalizeAngle(combinedAngle));
    sub_42EE00(self, nullptr, normalizedAngle, &rotationBits, position);

    void* widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, unsigned int*)>(
        CSF_AT(void**, widget, 0)[0x28 / 4])(widget, nullptr, position);

    struct MatrixHandle
    {
        std::uint32_t* matrix;
        unsigned char owned;
    };
    MatrixHandle handle;
    handle.matrix = RwMatrixCreate();
    handle.owned = 1;

    CSF_AT(float, handle.matrix, 0x28) = 1.0f;
    CSF_AT(float, handle.matrix, 0x14) = 1.0f;
    CSF_AT(float, handle.matrix, 0x00) = 1.0f;
    handle.matrix[4] = 0;
    handle.matrix[2] = 0;
    handle.matrix[1] = 0;
    handle.matrix[9] = 0;
    handle.matrix[8] = 0;
    handle.matrix[6] = 0;
    handle.matrix[14] = 0;
    handle.matrix[13] = 0;
    handle.matrix[12] = 0;
    handle.matrix[3] |= 0x20003u;

    float scale[2] = {0.0f, 0.0f};
    sub_42DE60(scale);
    TranslateMatrixHandle(&handle, nullptr, -halfWidth, -halfHeight, 0.0f);

    float rotation;
    std::memcpy(&rotation, &rotationBits, sizeof(rotation));
    RotateMatrixHandle(&handle, nullptr, rotation);
    ScaleMatrix(&handle, nullptr,
        Hud_FloatArgumentBits(scale[0]), Hud_FloatArgumentBits(scale[1]), 0);
    TranslateMatrixHandle(&handle, nullptr,
        scale[0] * halfWidth, scale[1] * halfHeight, 0.0f);

    widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, MatrixHandle*)>(
        CSF_AT(void**, widget, 0)[0x60 / 4])(widget, nullptr, &handle);

    widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    const auto* currentColor =
        reinterpret_cast<std::uint32_t*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, widget, 0)[0x6C / 4])(widget, nullptr);

    std::uint32_t color[4];
    color[0] = currentColor[0];
    color[1] = currentColor[1];
    color[2] = currentColor[2];
    color[3] = currentColor[3];
    color[3] = CSF_AT(std::uint32_t, orientation, 0x0C);

    widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, widget, 0)[0x68 / 4])(widget, nullptr, color);

    widget = CSF_AT(void*, reinterpret_cast<void*>(self->field_0064), 0);
    auto* result = reinterpret_cast<C_008342e8*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, widget, 0)[0x08 / 4])(widget, nullptr);

    if (handle.owned)
        return reinterpret_cast<C_008342e8*>(RwMatrixDestroy(handle.matrix));
    return result;
}

HOOK(0x0042F0E0, S_0042ee00_UpdateOrientationWidget, "auto");
