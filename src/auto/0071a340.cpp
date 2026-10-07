#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static double ConvertUnsignedSize(std::uint32_t value)
{
    double result = static_cast<double>(static_cast<std::int32_t>(value));
    if (static_cast<std::int32_t>(value) < 0)
        result += static_cast<double>(g_UnsignedIntegerBias);
    return result;
}

static int __fastcall C_00864508_SetSizeAndRelativeScale(
    C_00864508* self,
    void* /*edx*/,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t referenceWidth,
    std::uint32_t referenceHeight)
{
    std::uint32_t referenceSize[2] = {referenceWidth, referenceHeight};

    void* child = reinterpret_cast<void*>(self->field_0040);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, child, 0)[0xA0 / 4])(
            child, nullptr, referenceSize);

    child = reinterpret_cast<void*>(self->field_0040);
    self->field_0064 = width;
    self->field_0068 = height;
    void** childVtable = CSF_AT(void**, child, 0);

    const double convertedHeight = ConvertUnsignedSize(height);
    const double convertedReferenceHeight = ConvertUnsignedSize(referenceHeight);
    const float heightScale =
        static_cast<float>(convertedHeight / convertedReferenceHeight);

    const double convertedWidth = ConvertUnsignedSize(width);
    const double convertedReferenceWidth = ConvertUnsignedSize(referenceWidth);
    const float widthScale =
        static_cast<float>(convertedWidth / convertedReferenceWidth);

    return ((int(__fastcall*)(void*, void*, float, float))
        childVtable[0x150 / 4])(
            child, nullptr, widthScale, heightScale);
}

HOOK(0x0071A340, C_00864508_SetSizeAndRelativeScale, "auto");
