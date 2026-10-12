#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(int, __cdecl, GfxTeam_GetObjectBoundsData, 0x007F7BF0, (uint32_t* a1, uint32_t* a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectGetPosition, 0x007F8410, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(double, __cdecl, Rt2dObjectStringGetHeight, 0x007F9880, (int a1));
CSF_FUNC(double, __cdecl, GfxTeam_GetBrushWidth, 0x00802650, (int a1));
CSF_FUNC(double, __cdecl, Rt2dBrushGetHeight, 0x00802660, (int a1));

static S_00402350* __stdcall GfxTeam_GetPlayerBounds(
    S_00402350* output, int object)
{
    alignas(8) uint8_t transformBytes[0x3C];
    S_00402350& transform =
        *reinterpret_cast<S_00402350*>(transformBytes);

    // Address identities listed for the two constructor-like callees.
    (void)CSF_VTBL(0x00402350);
    (void)CSF_VTBL(0x006ABB80);

    S00402350_InitTransform(&transform, nullptr);

    void* typeDescriptor =
        CSF_AT(void*, reinterpret_cast<void*>(object), 0x5C);
    const int16_t objectType = CSF_AT(int16_t, typeDescriptor, 0);

    switch (objectType)
    {
    case 2:
    {
        const float height =
            static_cast<float>(Rt2dObjectStringGetHeight(object));
        std::memcpy(&transform.field_000c, &height, sizeof(height));
        std::memcpy(&transform.field_0010, &height, sizeof(height));
        std::memcpy(&transform.field_0014, &height, sizeof(height));
        break;
    }

    case 5:
    {
        alignas(8) uint8_t positionBytes[0x14];
        CSF_AT(float, positionBytes, 0x00) = 0.0f;
        CSF_AT(float, positionBytes, 0x04) = 0.0f;
        CSF_AT(float, positionBytes, 0x08) = 0.0f;

        Rt2dObjectGetPosition(
            reinterpret_cast<uint32_t*>(object),
            reinterpret_cast<uint32_t*>(positionBytes));

        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
            CSF_AT(float, positionBytes, 0x00),
            -CSF_AT(float, positionBytes, 0x00),
            CSF_AT(float, positionBytes, 0x04),
            -CSF_AT(float, positionBytes, 0x04),
            CSF_AT(float, positionBytes, 0x08),
            -CSF_AT(float, positionBytes, 0x08));
        break;
    }

    case 3:
    {
        const float height =
            static_cast<float>(Rt2dObjectStringGetHeight(object));
        const float combinedHeight =
            static_cast<float>(Rt2dBrushGetHeight(object) + height);

        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
            height, -height, height, -height,
            combinedHeight, -combinedHeight);
        break;
    }

    case 6:
    {
        const float width =
            static_cast<float>(GfxTeam_GetBrushWidth(object));
        const float height =
            static_cast<float>(Rt2dBrushGetHeight(object));

        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
            width, -width, width, -width, height, -height);
        break;
    }

    case 8:
    {
        alignas(8) uint8_t boundsBytes[0x20];
        GfxTeam_GetObjectBoundsData(
            reinterpret_cast<uint32_t*>(object), nullptr, 0,
            reinterpret_cast<int>(boundsBytes));

        S_006abb80_InitializeBounds(
            reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
            CSF_AT(float, boundsBytes, 0x00),
            CSF_AT(float, boundsBytes, 0x10),
            CSF_AT(float, boundsBytes, 0x04),
            CSF_AT(float, boundsBytes, 0x14),
            CSF_AT(float, boundsBytes, 0x08),
            CSF_AT(float, boundsBytes, 0x18));
        break;
    }

    default:
        break;
    }

    std::memcpy(output, transformBytes, 0x3C);
    return output;
}

HOOK(0x00700250, GfxTeam_GetPlayerBounds, "auto");
