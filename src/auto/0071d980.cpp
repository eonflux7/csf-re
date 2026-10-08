#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned char*, __fastcall, FindProperty, 0x006932B0,
         (unsigned char* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940,
         (unsigned char* node, void* edx, char* name));

CSF_GLOBAL(char*, g_FirstScalarProperty, 0x008A8774);
CSF_GLOBAL(char*, g_SecondScalarProperty, 0x008A8778);

static void __fastcall GuiBuilder_LoadScalarProperties(
    void* self, void* /*edx*/, unsigned char* node)
{
    if (!node)
        return;

    if (FindProperty(node, nullptr, g_FirstScalarProperty))
        CSF_AT(float, self, 0x38) =
            static_cast<float>(ReadFloatProperty(node, nullptr, g_FirstScalarProperty));

    if (FindProperty(node, nullptr, g_SecondScalarProperty))
        CSF_AT(float, self, 0x3C) =
            static_cast<float>(ReadFloatProperty(node, nullptr, g_SecondScalarProperty));
}

HOOK(0x0071D980, GuiBuilder_LoadScalarProperties, "auto");
