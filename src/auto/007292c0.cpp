#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00863f60_ForwardScaleToAttachedObject,
         0x00713E60, (C_00863f60* self, void* edx, int xScaleBits, int yScaleBits));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int32_t RoundScaledCoordinate(float value)
{
    // The x87 C0 flag is also set for an unordered comparison.
    const double adjusted = !(value >= g_fZero)
        ? static_cast<double>(value) - static_cast<double>(g_HalfDurationScale)
        : static_cast<double>(value) + static_cast<double>(g_HalfDurationScale);

    // __ftol2 produces a 64-bit integer; only its low DWORD is used.
    return static_cast<int32_t>(
        static_cast<uint32_t>(static_cast<int64_t>(adjusted)));
}

static int __fastcall C_008658f8_ScaleOwnedGeometry(
    C_008658f8* self, void* /*edx*/, float xScale, float yScale)
{
    int xScaleBits;
    int yScaleBits;
    std::memcpy(&xScaleBits, &xScale, sizeof(xScaleBits));
    std::memcpy(&yScaleBits, &yScale, sizeof(yScaleBits));
    C_00863f60_ForwardScaleToAttachedObject(
        self, nullptr, xScaleBits, yScaleBits);

    void* child = reinterpret_cast<void*>(self->field_0030);
    if (!child)
        return 0;

    // The first push edi saves EDI; it is not a virtual-call argument.
    auto* coordinates =
        ((int32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[8])(child, nullptr);
    const float scaledPositionX = static_cast<float>(
        static_cast<double>(CSF_AT(int32_t, coordinates, 0)) * xScale);

    child = reinterpret_cast<void*>(self->field_0030);
    coordinates =
        ((int32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[8])(child, nullptr);
    const float scaledPositionY = static_cast<float>(
        static_cast<double>(CSF_AT(int32_t, coordinates, 4)) * yScale);

    child = reinterpret_cast<void*>(self->field_0030);
    coordinates =
        ((int32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[10])(child, nullptr);
    const float scaledSizeX = static_cast<float>(
        static_cast<double>(CSF_AT(int32_t, coordinates, 0)) * xScale);

    child = reinterpret_cast<void*>(self->field_0030);
    coordinates =
        ((int32_t* (__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[10])(child, nullptr);
    const float scaledSizeY = static_cast<float>(
        static_cast<double>(CSF_AT(int32_t, coordinates, 4)) * yScale);

    const int32_t positionY = RoundScaledCoordinate(scaledPositionY);
    const int32_t positionX = RoundScaledCoordinate(scaledPositionX);
    int32_t position[2] = {positionX, positionY};

    child = reinterpret_cast<void*>(self->field_0030);
    ((void (__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, child, 0)[7])(child, nullptr, position);

    const int32_t sizeY = RoundScaledCoordinate(scaledSizeY);
    const int32_t sizeX = RoundScaledCoordinate(scaledSizeX);
    int32_t size[2] = {sizeX, sizeY};

    child = reinterpret_cast<void*>(self->field_0030);
    return ((int (__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, child, 0)[9])(child, nullptr, size);
}

HOOK(0x007292C0, C_008658f8_ScaleOwnedGeometry, "auto");
