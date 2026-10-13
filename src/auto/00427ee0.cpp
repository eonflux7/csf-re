#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ObjectiveBannerHeightScale, 0x0082FEC8);

static int __fastcall S_00427bf0_ResetObjectiveBanner(
    S_00427bf0* self, void* /*edx*/)
{
    alignas(8) uint8_t colorBytes[0x10];
    alignas(8) uint8_t positionBytes[0x8];
    alignas(8) uint8_t sizeBytes[0xC];

    void* referenceWidget = reinterpret_cast<void*>(self->field_0018);
    auto* referenceSize =
        ((uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, referenceWidget, 0)[0x28 / 4])(
                referenceWidget, nullptr);
    const uint32_t referenceHeight = referenceSize[1];
    const uint32_t scaledHeight = static_cast<uint32_t>(
        static_cast<int64_t>(
            static_cast<double>(referenceHeight) *
            g_ObjectiveBannerHeightScale));

    referenceWidget = reinterpret_cast<void*>(self->field_0018);
    auto* referencePosition =
        ((uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, referenceWidget, 0)[0x20 / 4])(
                referenceWidget, nullptr);
    CSF_AT(uint32_t, positionBytes, 0) = referencePosition[0];
    CSF_AT(uint32_t, positionBytes, 4) =
        referencePosition[1] - ((scaledHeight - referenceHeight) >> 1);
    CSF_AT(uint32_t, sizeBytes, 0) = 0;
    CSF_AT(uint32_t, sizeBytes, 4) = 0;

    void* screenObject = LookupObjectById(
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())),
        nullptr);
    auto* screenDimensions =
        ((uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, screenObject, 0)[0x18 / 4])(
                screenObject, nullptr);
    CSF_AT(uint32_t, positionBytes, 0) = 0u - screenDimensions[0];

    screenObject = LookupObjectById(
        reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt())),
        nullptr);
    screenDimensions =
        ((uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, screenObject, 0)[0x18 / 4])(
                screenObject, nullptr);
    const uint32_t bannerWidth = 3u * screenDimensions[0];
    void* bannerWidget = reinterpret_cast<void*>(self->field_0020);
    CSF_AT(uint32_t, sizeBytes, 0) = bannerWidth;
    CSF_AT(uint32_t, sizeBytes, 4) = scaledHeight;

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, bannerWidget, 0)[0x28 / 4])(
            bannerWidget, nullptr, positionBytes);

    bannerWidget = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, bannerWidget, 0)[0x38 / 4])(
            bannerWidget, nullptr, sizeBytes);

    bannerWidget = reinterpret_cast<void*>(self->field_0020);
    self->field_0044 = 0;
    auto* color =
        ((uint32_t*(__fastcall*)(void*, void*))
            CSF_AT(void**, bannerWidget, 0)[0x6C / 4])(
                bannerWidget, nullptr);
    CSF_AT(uint32_t, colorBytes, 0) = color[0];
    CSF_AT(uint32_t, colorBytes, 4) = color[1];
    CSF_AT(uint32_t, colorBytes, 8) = color[2];

    bannerWidget = reinterpret_cast<void*>(self->field_0020);
    CSF_AT(uint32_t, colorBytes, 0xC) = 0;
    const int result =
        ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, bannerWidget, 0)[0x68 / 4])(
                bannerWidget, nullptr, colorBytes);
    self->field_0000 = 0;
    return result;
}

HOOK(0x00427EE0, S_00427bf0_ResetObjectiveBanner, "auto");
