#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

static CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
                (std::uint8_t* self, void* edx, char* name));
static CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910,
                (std::uint8_t* self, void* edx, char* name));
static CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940,
                (std::uint8_t* self, void* edx, char* name));

static CSF_GLOBAL(char*, g_Property20, 0x008A8758);
static CSF_GLOBAL(char*, g_Property24, 0x008A875C);
static CSF_GLOBAL(char*, g_Property28, 0x008A8760);
static CSF_GLOBAL(char*, g_Property2C, 0x008A8764);
static CSF_GLOBAL(char*, g_Property30, 0x008A8768);
static CSF_GLOBAL(char*, g_Property34, 0x008A876C);

static void __fastcall LoadGuiScalarProperties(
    void* self, void* /*edx*/, std::uint8_t* properties)
{
    if (!properties)
        return;

    if (FindProperty(properties, nullptr, g_Property20))
        CSF_AT(float, self, 0x20) =
            static_cast<float>(ReadFloatProperty(properties, nullptr, g_Property20));

    if (FindProperty(properties, nullptr, g_Property24))
        CSF_AT(float, self, 0x24) =
            static_cast<float>(ReadFloatProperty(properties, nullptr, g_Property24));

    if (FindProperty(properties, nullptr, g_Property28))
        CSF_AT(float, self, 0x28) =
            static_cast<float>(ReadFloatProperty(properties, nullptr, g_Property28));

    if (FindProperty(properties, nullptr, g_Property2C))
        CSF_AT(float, self, 0x2C) =
            static_cast<float>(ReadFloatProperty(properties, nullptr, g_Property2C));

    if (FindProperty(properties, nullptr, g_Property30))
        CSF_AT(int, self, 0x30) =
            ReadIntegerProperty(properties, nullptr, g_Property30);

    if (FindProperty(properties, nullptr, g_Property34))
        CSF_AT(float, self, 0x34) =
            static_cast<float>(ReadFloatProperty(properties, nullptr, g_Property34));
}

HOOK(0x0071D890, LoadGuiScalarProperties, "auto");
