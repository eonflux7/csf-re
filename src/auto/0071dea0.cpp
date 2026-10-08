#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x006932B0: thiscall (node, name) -> property/component block, pops 4.
CSF_FUNC(unsigned char*, __fastcall, FindProperty, 0x006932B0,
         (unsigned char* node, void* edx, char* name));

// 0x00692BD0: thiscall (block, index) -> component handle, pops 4.
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));

// 0x00692CC0: fastcall (handle) -> double value in st(0), pops 0.
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0,
         (std::uint32_t component));

CSF_GLOBAL(char*, g_TwoComponentProperty2, 0x008A85F8);

// Reads the first two components of a node's property block and stores them
// as floats into destination[0] and destination[1]. Returns 1 on success.
static unsigned char __cdecl GuiBuilder_ReadTwoFloatComponents(
    float* destination, unsigned char* node)
{
    if (!node)
        return 0;

    unsigned char* components = FindProperty(node, nullptr, g_TwoComponentProperty2);
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

HOOK(0x0071DEA0, GuiBuilder_ReadTwoFloatComponents, "auto");
