#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(const char*, g_pszKey_TOP_CGuiControl, 0x00899634);
CSF_GLOBAL(const char*, g_pszKey_LEFT_CGuiControl, 0x00899638);
CSF_GLOBAL(const char*, g_pszKey_WIDTH_CGuiControl, 0x0089963C);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT_CGuiControl, 0x00899640);
CSF_GLOBAL(const char*, g_pszKey_TEX_V0_CGuiControl, 0x00899644);
CSF_GLOBAL(const char*, g_pszKey_TEX_U0_CGuiControl, 0x00899648);
CSF_GLOBAL(const char*, g_pszKey_TEX_V1_CGuiControl, 0x0089964C);
CSF_GLOBAL(const char*, g_pszKey_TEX_U1_CGuiControl, 0x00899650);

static void __fastcall HudSpritesLayout_ReadSpriteLayout(
    std::uint8_t* self, void* /*edx*/, S_00692b80* properties)
{
    auto* source = reinterpret_cast<std::uint8_t*>(properties);

    CSF_AT(int, self, 0x138) =
        ReadIntegerProperty(source, nullptr, const_cast<char*>(g_pszKey_TOP_CGuiControl));
    CSF_AT(int, self, 0x13C) =
        ReadIntegerProperty(source, nullptr, const_cast<char*>(g_pszKey_LEFT_CGuiControl));
    CSF_AT(int, self, 0x140) =
        ReadIntegerProperty(source, nullptr, const_cast<char*>(g_pszKey_WIDTH_CGuiControl));
    CSF_AT(int, self, 0x144) =
        ReadIntegerProperty(source, nullptr, const_cast<char*>(g_pszKey_HEIGHT_CGuiControl));

    CSF_AT(float, self, 0x148) = static_cast<float>(
        ReadFloatProperty(source, nullptr, const_cast<char*>(g_pszKey_TEX_V0_CGuiControl)));
    CSF_AT(float, self, 0x14C) = static_cast<float>(
        ReadFloatProperty(source, nullptr, const_cast<char*>(g_pszKey_TEX_U0_CGuiControl)));
    CSF_AT(float, self, 0x150) = static_cast<float>(
        ReadFloatProperty(source, nullptr, const_cast<char*>(g_pszKey_TEX_V1_CGuiControl)));
    CSF_AT(float, self, 0x154) = static_cast<float>(
        ReadFloatProperty(source, nullptr, const_cast<char*>(g_pszKey_TEX_U1_CGuiControl)));
}

HOOK(0x00436790, HudSpritesLayout_ReadSpriteLayout, "auto");
