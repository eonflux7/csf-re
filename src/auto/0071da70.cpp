#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970,
         (std::uint8_t* node, void* edx, char* name));

CSF_GLOBAL(char*, g_Component3Attribute, 0x008A8630);
CSF_GLOBAL(char*, g_Component0Attribute, 0x008A8634);
CSF_GLOBAL(char*, g_Component1Attribute, 0x008A8638);
CSF_GLOBAL(char*, g_Component2Attribute, 0x008A863C);

static char __stdcall CGuiBuilder_ReadFourComponents(
    std::uint8_t* node, float* output)
{
    if (!node)
        return 0;

    output[3] = static_cast<float>(
        ReadIntegerAttribute(node, nullptr, g_Component3Attribute));
    output[0] = static_cast<float>(
        ReadIntegerAttribute(node, nullptr, g_Component0Attribute));
    output[1] = static_cast<float>(
        ReadIntegerAttribute(node, nullptr, g_Component1Attribute));
    output[2] = static_cast<float>(
        ReadIntegerAttribute(node, nullptr, g_Component2Attribute));

    return 1;
}

HOOK(0x0071DA70, CGuiBuilder_ReadFourComponents, "auto");
