#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (std::uint8_t* self, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40,
         (unsigned int component));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910,
         (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0,
         (std::uint8_t* self, void* edx, char* name, char* buffer, int capacity));
CSF_FUNC(char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70,
         (std::uint8_t* property, float* components));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_DefaultColorRed, 0x008A7DB0);
CSF_GLOBAL(float, g_DefaultColorGreen, 0x008A7DB4);
CSF_GLOBAL(float, g_DefaultColorBlue, 0x008A7DB8);
CSF_GLOBAL(float, g_DefaultColorAlpha, 0x008A7DBC);
CSF_GLOBAL(char*, g_PositionPropertyName, 0x008A85EC);
CSF_GLOBAL(char*, g_SizePropertyName, 0x008A85F0);
CSF_GLOBAL(char*, g_TextPropertyName, 0x008A8600);
CSF_GLOBAL(char*, g_ColorPropertyName, 0x008A8628);
CSF_GLOBAL(char*, g_TextModePropertyName, 0x008A872C);
CSF_GLOBAL(char*, g_InheritColorPropertyName, 0x008A8780);

static void __fastcall CGuiBuilder_ApplyTextProperties(
    void* self, void* /*edx*/, std::uint8_t* properties, void* target)
{
    if (!properties || !target)
        return;

    float color[4];
    color[0] = g_DefaultColorRed * g_ColorChannelScale;
    color[1] = g_DefaultColorGreen * g_ColorChannelScale;
    color[2] = g_DefaultColorBlue * g_ColorChannelScale;
    color[3] = g_DefaultColorAlpha * g_ColorChannelScale;

    if (FindProperty(properties, nullptr, g_InheritColorPropertyName))
    {
        // Copy the embedded color's raw bits, as in the original.
        CSF_AT(std::uint32_t, color, 0x00) = CSF_AT(std::uint32_t, self, 0x60);
        CSF_AT(std::uint32_t, color, 0x04) = CSF_AT(std::uint32_t, self, 0x64);
        CSF_AT(std::uint32_t, color, 0x08) = CSF_AT(std::uint32_t, self, 0x68);
        CSF_AT(std::uint32_t, color, 0x0C) = CSF_AT(std::uint32_t, self, 0x6C);
    }
    else
    {
        auto* property = FindProperty(properties, nullptr, g_ColorPropertyName);
        CGuiBuilder_ReadFourComponents(property, color);
    }

    std::uint32_t x = 0;
    std::uint32_t y = 0;
    auto* position = FindProperty(properties, nullptr, g_PositionPropertyName);
    if (position)
    {
        const auto first = GetComponent(position, nullptr, 0);
        const auto second = GetComponent(position, nullptr, 1);
        y = static_cast<std::uint32_t>(ParseIntegerComponent(second));
        x = static_cast<std::uint32_t>(ParseIntegerComponent(first));
    }

    std::uint32_t width = 0;
    std::uint32_t height = 0;
    auto* size = FindProperty(properties, nullptr, g_SizePropertyName);
    if (size)
    {
        const auto first = GetComponent(size, nullptr, 0);
        const auto second = GetComponent(size, nullptr, 1);
        height = static_cast<std::uint32_t>(ParseIntegerComponent(second));
        width = static_cast<std::uint32_t>(ParseIntegerComponent(first));
    }

    std::uint32_t rectangle[4];
    rectangle[0] = x;
    rectangle[2] = x + width;
    char text[256] = {};
    rectangle[3] = y + height;
    rectangle[1] = y;

    auto* textProperty = FindProperty(properties, nullptr, g_TextPropertyName);
    if (textProperty)
        MenuExtras_ReadText(textProperty, nullptr, g_TextPropertyName, text, 256);

    int textMode = 2;
    if (FindProperty(properties, nullptr, g_TextModePropertyName))
        textMode = ReadIntegerProperty(properties, nullptr, g_TextModePropertyName);

    reinterpret_cast<void (__fastcall*)(void*, void*, float*, int, char*)>(
        CSF_AT(void**, target, 0)[0x200 / 4])(
            target, nullptr, color, textMode, text);

    reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t*, char*)>(
        CSF_AT(void**, target, 0)[0x208 / 4])(
            target, nullptr, rectangle, text);
}

HOOK(0x0071E780, CGuiBuilder_ApplyTextProperties, "auto");
