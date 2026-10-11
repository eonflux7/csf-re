#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int, __fastcall, GfxSkin_SetRenderOption, 0x006C4B00, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, GfxSkin_UpdateObjectRendering, 0x006C6C00, (void* self, void* edx, void* a2));

static unsigned int __fastcall C_00860288_ConfigureObjectRenderRegions(
    C_00860288* self, void* /*edx*/, C_0085e7b8* object)
{
    using FlagsGetter = std::uint8_t* (__fastcall*)(void*, void*);
    using SettingsGetter = C_008670f0* (__fastcall*)(void*, void*);

    void* renderHandle = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(RenderHandle_FromObject(object, nullptr)));

    std::uint8_t* flags =
        reinterpret_cast<FlagsGetter>(CSF_AT(void**, object, 0)[0x20 / 4])(
            object, nullptr);

    int region;
    if ((CSF_AT(std::uint8_t, flags, 0) & 8) != 0)
    {
        region = GetSceneRegionAt(
            reinterpret_cast<void*>(self->field_0004), nullptr, 5);
    }
    else
    {
        flags =
            reinterpret_cast<FlagsGetter>(CSF_AT(void**, object, 0)[0x20 / 4])(
                object, nullptr);

        if ((CSF_AT(std::uint8_t, flags, 0) & 0x10) != 0)
        {
            region = GetSceneRegionAt(
                reinterpret_cast<void*>(self->field_0004), nullptr, 4);
        }
        else
        {
            flags =
                reinterpret_cast<FlagsGetter>(CSF_AT(void**, object, 0)[0x20 / 4])(
                    object, nullptr);

            if ((CSF_AT(std::uint8_t, flags, 0) & 1) != 0)
            {
                region = GetSceneRegionAt(
                    reinterpret_cast<void*>(self->field_0004), nullptr, 9);
            }
            else
            {
                region = GetSceneRegionAt(
                    reinterpret_cast<void*>(self->field_0004), nullptr, 8);
            }
        }
    }

    C_008670f0* settings =
        reinterpret_cast<SettingsGetter>(CSF_AT(void**, renderHandle, 0)[0x1C / 4])(
            renderHandle, nullptr);
    C_008670f0_SetField0C(settings, nullptr, region);

    flags =
        reinterpret_cast<FlagsGetter>(CSF_AT(void**, object, 0)[0x20 / 4])(
            object, nullptr);
    const bool hasFlag2000 =
        (CSF_AT(std::uint32_t, flags, 0) & 0x2000) == 0x2000;

    flags =
        reinterpret_cast<FlagsGetter>(CSF_AT(void**, object, 0)[0x20 / 4])(
            object, nullptr);
    const bool hasFlag1000 =
        (CSF_AT(std::uint32_t, flags, 0) & 0x1000) == 4096;

    if (hasFlag2000 || hasFlag1000)
    {
        // The original replaces only the low byte of the object argument.
        const int optionValue = static_cast<int>(
            (reinterpret_cast<std::uintptr_t>(object) & ~std::uint32_t{0xFF}) |
            static_cast<std::uint32_t>(hasFlag1000));

        settings =
            reinterpret_cast<SettingsGetter>(CSF_AT(void**, renderHandle, 0)[0x1C / 4])(
                renderHandle, nullptr);
        GfxSkin_SetRenderOption(settings, nullptr, 4, optionValue);
    }

    return GfxSkin_UpdateObjectRendering(self, nullptr, object);
}

HOOK(0x006C6C60, C_00860288_ConfigureObjectRenderRegions, "auto");
