#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00865758_DrawExpandedOverlay(
    C_00865758* self, void* /*edx*/)
{
    if (self->field_0030 == 0)
        return 0;

    void* referenceObject =
        reinterpret_cast<void* (__fastcall*)(C_00865758*, void*)>(
            self->vtbl[0x18 / 4])(self, nullptr);

    if (referenceObject == nullptr)
        return 0;

    std::uint32_t position[2];
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, referenceObject, 0)[0x160 / 4])(
            referenceObject, nullptr, position);

    position[0] -= 2u;
    position[1] -= 2u;

    void* referenceSize =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, referenceObject, 0)[0xA4 / 4])(
                referenceObject, nullptr);

    std::uint32_t size[2];
    size[0] = CSF_AT(std::uint32_t, referenceSize, 0x00);
    size[1] = CSF_AT(std::uint32_t, referenceSize, 0x04);
    size[0] += 4u;
    size[1] += 4u;

    void* overlay = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, overlay, 0)[0x1C / 4])(
            overlay, nullptr, position);

    overlay = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
        CSF_AT(void**, overlay, 0)[0x24 / 4])(
            overlay, nullptr, size);

    overlay = reinterpret_cast<void*>(self->field_0030);
    void* colorData =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, overlay, 0)[0x30 / 4])(overlay, nullptr);

    struct ColorState {
        std::uint32_t channels[3];
        float alpha;
    };

    ColorState savedColor;
    savedColor.channels[0] = CSF_AT(std::uint32_t, colorData, 0x00);
    savedColor.channels[1] = CSF_AT(std::uint32_t, colorData, 0x04);
    savedColor.channels[2] = CSF_AT(std::uint32_t, colorData, 0x08);
    savedColor.alpha = CSF_AT(float, colorData, 0x0C);

    ColorState scaledColor = savedColor;
    scaledColor.alpha = static_cast<float>(
        static_cast<double>(savedColor.alpha) *
        static_cast<double>(self->field_0034));

    overlay = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<int (__fastcall*)(void*, void*, ColorState*)>(
        CSF_AT(void**, overlay, 0)[0x2C / 4])(
            overlay, nullptr, &scaledColor);

    overlay = reinterpret_cast<void*>(self->field_0030);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, overlay, 0)[0x08 / 4])(overlay, nullptr);

    overlay = reinterpret_cast<void*>(self->field_0030);
    return reinterpret_cast<int (__fastcall*)(void*, void*, ColorState*)>(
        CSF_AT(void**, overlay, 0)[0x2C / 4])(
            overlay, nullptr, &savedColor);
}

HOOK(0x00728D00, C_00865758_DrawExpandedOverlay, "auto");
