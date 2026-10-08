#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindAttribute, 0x006932B0,
         (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990,
         (std::uint8_t* node, void* edx, char* name));

CSF_GLOBAL(char*, g_FloatAttribute, 0x008A8718);
CSF_GLOBAL(float, g_Zero, 0x0082D4CC);

static int __stdcall CGuiBuilder_ReadNonzeroFloatAttribute(
    std::uint8_t* node, float* output)
{
    if (!node)
        return 0;

    *output = 0.0f;
    if (FindAttribute(node, nullptr, g_FloatAttribute))
        *output = static_cast<float>(
            ReadFloatAttribute(node, nullptr, g_FloatAttribute));

    return *output != g_Zero;
}

HOOK(0x0071D9E0, CGuiBuilder_ReadNonzeroFloatAttribute, "auto");
