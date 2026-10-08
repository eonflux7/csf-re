#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned char*, __fastcall, FindProperty, 0x006932B0,
         (unsigned char* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910,
         (unsigned char* node, void* edx, char* name));
CSF_FUNC(unsigned char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70,
         (unsigned char* node, float* destination));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);
CSF_GLOBAL(float, g_DefaultColorRed, 0x008A7DB0);
CSF_GLOBAL(float, g_DefaultColorGreen, 0x008A7DB4);
CSF_GLOBAL(float, g_DefaultColorBlue, 0x008A7DB8);
CSF_GLOBAL(float, g_DefaultColorAlpha, 0x008A7DBC);

CSF_GLOBAL(char*, g_StoredColorProperty, 0x008A878C);
CSF_GLOBAL(char*, g_InlineColorProperty, 0x008A8628);
CSF_GLOBAL(char*, g_ColorModeProperty, 0x008A872C);
CSF_GLOBAL(char*, g_EnableFlagProperty, 0x008A8720);

static void __fastcall GuiBuilder_ApplyColorProperties(
    void* self, void* /*edx*/, unsigned char* node, void* target)
{
    if (!node || !target)
        return;

    char* storedColorProperty = g_StoredColorProperty;
    float color[4];
    color[0] = g_DefaultColorRed * g_ColorChannelScale;
    color[1] = g_DefaultColorGreen * g_ColorChannelScale;
    color[2] = g_DefaultColorBlue * g_ColorChannelScale;
    color[3] = g_DefaultColorAlpha * g_ColorChannelScale;

    if (FindProperty(node, nullptr, storedColorProperty))
    {
        std::uint32_t bits[4];
        bits[0] = CSF_AT(std::uint32_t, self, 0x70);
        bits[1] = CSF_AT(std::uint32_t, self, 0x74);
        bits[2] = CSF_AT(std::uint32_t, self, 0x78);
        bits[3] = CSF_AT(std::uint32_t, self, 0x7C);
        std::memcpy(color, bits, sizeof(color));
    }
    else
    {
        unsigned char* property = FindProperty(node, nullptr, g_InlineColorProperty);
        CGuiBuilder_ReadFourComponents(property, color);
    }

    int colorMode = 2;
    if (FindProperty(node, nullptr, g_ColorModeProperty))
        colorMode = ReadIntegerProperty(node, nullptr, g_ColorModeProperty);

    ((void(__fastcall*)(void*, void*, float*, int, int))
        CSF_AT(void**, target, 0)[0x1F8 / 4])(
            target, nullptr, color, colorMode, 0);

    if (FindProperty(node, nullptr, g_EnableFlagProperty))
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, target, 0)[0x1FC / 4])(
                target, nullptr, 1);
    }
}

HOOK(0x0071EAC0, GuiBuilder_ApplyColorProperties, "auto");
