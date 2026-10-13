#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (S_00692b80* self, void* , char* attributeName));

static int __fastcall HudObjectives_ApplyColorProperty(
    void* self, void* /*edx*/, S_00692b80* properties,
    S_00692b80* propertyName, std::uint32_t* defaultColor)
{
    void* owner = CSF_AT(void*, self, 0x04);
    void* target =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr);

    if (!target)
    {
        CSF_AT(std::uint8_t, self, 0x08) = 0;
        return 0;
    }

    char* name = reinterpret_cast<char*>(propertyName);
    if (properties && S_00692b80_FindProperty(properties, nullptr, name))
    {
        S_00692b80* property =
            S_00692b80_FindProperty(properties, nullptr, name);

        alignas(8) std::uint8_t colorBytes[0x10];
        CSF_AT(float, colorBytes, 0x00) = 0.0f;
        CSF_AT(float, colorBytes, 0x04) = 0.0f;
        CSF_AT(float, colorBytes, 0x08) = 0.0f;
        CSF_AT(float, colorBytes, 0x0C) = 1.0f;

        CSF_AT(float, colorBytes, 0x0C) = static_cast<float>(
            ReadIntegerAttribute(property, nullptr,
                const_cast<char*>(CSF_STR(0x0082FEB8, ".A"))));
        CSF_AT(float, colorBytes, 0x00) = static_cast<float>(
            ReadIntegerAttribute(property, nullptr,
                const_cast<char*>(CSF_STR(0x0082FEB4, ".R"))));
        CSF_AT(float, colorBytes, 0x04) = static_cast<float>(
            ReadIntegerAttribute(property, nullptr,
                const_cast<char*>(CSF_STR(0x0082FEB0, ".G"))));
        CSF_AT(float, colorBytes, 0x08) = static_cast<float>(
            ReadIntegerAttribute(property, nullptr,
                const_cast<char*>(CSF_STR(0x0082FEAC, ".B"))));

        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, target, 0)[0x2C / 4])(
                target, nullptr, reinterpret_cast<float*>(colorBytes));

        const std::uint32_t red = CSF_AT(std::uint32_t, colorBytes, 0x00);
        const std::uint32_t green = CSF_AT(std::uint32_t, colorBytes, 0x04);
        const std::uint32_t blue = CSF_AT(std::uint32_t, colorBytes, 0x08);
        CSF_AT(std::uint32_t, self, 0x48) = red;
        const std::uint32_t alpha = CSF_AT(std::uint32_t, colorBytes, 0x0C);
        CSF_AT(std::uint32_t, self, 0x4C) = green;
        CSF_AT(std::uint32_t, self, 0x50) = blue;
        CSF_AT(std::uint32_t, self, 0x54) = alpha;
    }
    else
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*)>(
            CSF_AT(void**, target, 0)[0x2C / 4])(
                target, nullptr, defaultColor);

        CSF_AT(std::uint32_t, self, 0x48) = defaultColor[0];
        CSF_AT(std::uint32_t, self, 0x4C) = defaultColor[1];
        CSF_AT(std::uint32_t, self, 0x50) = defaultColor[2];
        CSF_AT(std::uint32_t, self, 0x54) = defaultColor[3];
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(target));
}

HOOK(0x00427860, HudObjectives_ApplyColorProperty, "auto");
