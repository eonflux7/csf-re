#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned char*, __fastcall, FindProperty, 0x006932B0,
         (unsigned char* node, void* edx, char* name));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0,
         (std::uint32_t component));

CSF_GLOBAL(char*, g_TwoComponentProperty, 0x008A85F4);

static unsigned char __cdecl GuiBuilder_ReadTwoComponents(
    float* destination, unsigned char* node)
{
    if (!node)
        return 0;

    unsigned char* components = FindProperty(node, nullptr, g_TwoComponentProperty);
    if (!components)
        return 0;

    const std::uint32_t first = GetComponent(components, nullptr, 0);
    CSF_AT(float, destination, 0x00) =
        static_cast<float>(ReadComponentFloat(first));

    const std::uint32_t second = GetComponent(components, nullptr, 1);
    CSF_AT(float, destination, 0x04) =
        static_cast<float>(ReadComponentFloat(second));

    return 1;
}

HOOK(0x0071DE40, GuiBuilder_ReadTwoComponents, "auto");
