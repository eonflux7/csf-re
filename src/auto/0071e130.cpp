#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned char*, __fastcall, FindProperty, 0x006932B0,
         (unsigned char* node, void* edx, char* name));
CSF_FUNC(unsigned char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70,
         (unsigned char* node, float* destination));

CSF_GLOBAL(char*, g_ColorPropertyAt60, 0x008A8780);
CSF_GLOBAL(char*, g_ColorPropertyAt40, 0x008A8784);
CSF_GLOBAL(char*, g_ColorPropertyAt50, 0x008A8788);
CSF_GLOBAL(char*, g_ColorPropertyAt70, 0x008A878C);

static void __fastcall GuiBuilder_LoadColorProperties(
    void* self, void* /*edx*/, unsigned char* node)
{
    if (!node)
        return;

    float components[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    std::uint32_t bits[4];

    unsigned char* property = FindProperty(node, nullptr, g_ColorPropertyAt60);
    if (CGuiBuilder_ReadFourComponents(property, components))
    {
        std::memcpy(bits, components, sizeof(bits));
        CSF_AT(std::uint32_t, self, 0x60) = bits[0];
        CSF_AT(std::uint32_t, self, 0x64) = bits[1];
        CSF_AT(std::uint32_t, self, 0x68) = bits[2];
        CSF_AT(std::uint32_t, self, 0x6C) = bits[3];
    }

    property = FindProperty(node, nullptr, g_ColorPropertyAt40);
    if (CGuiBuilder_ReadFourComponents(property, components))
    {
        std::memcpy(bits, components, sizeof(bits));
        CSF_AT(std::uint32_t, self, 0x40) = bits[0];
        CSF_AT(std::uint32_t, self, 0x44) = bits[1];
        CSF_AT(std::uint32_t, self, 0x48) = bits[2];
        CSF_AT(std::uint32_t, self, 0x4C) = bits[3];
    }

    property = FindProperty(node, nullptr, g_ColorPropertyAt50);
    if (CGuiBuilder_ReadFourComponents(property, components))
    {
        std::memcpy(bits, components, sizeof(bits));
        CSF_AT(std::uint32_t, self, 0x50) = bits[0];
        CSF_AT(std::uint32_t, self, 0x54) = bits[1];
        CSF_AT(std::uint32_t, self, 0x58) = bits[2];
        CSF_AT(std::uint32_t, self, 0x5C) = bits[3];
    }

    property = FindProperty(node, nullptr, g_ColorPropertyAt70);
    if (CGuiBuilder_ReadFourComponents(property, components))
    {
        std::memcpy(bits, components, sizeof(bits));
        CSF_AT(std::uint32_t, self, 0x70) = bits[0];
        CSF_AT(std::uint32_t, self, 0x74) = bits[1];
        CSF_AT(std::uint32_t, self, 0x78) = bits[2];
        CSF_AT(std::uint32_t, self, 0x7C) = bits[3];
    }
}

HOOK(0x0071E130, GuiBuilder_LoadColorProperties, "auto");
