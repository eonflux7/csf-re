#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SetSubtitlePanelPosition, 0x004F1CE0, (void* self, void* edx, float x, float y));
CSF_FUNC(int, __fastcall, HudStringShow_LoadTextureDimensions, 0x004F1DC0, (void* self, void* , S_00692b80* properties));
CSF_FUNC(char, __fastcall, S_004f41e0_LoadGlobalContext, 0x004F4370, (S_004f41e0* self, void* , S_00692b80* node));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(const char*, g_pszKey_CONTEXTO_EVENTOS, 0x008998F0);
CSF_GLOBAL(const char*, g_pszKey_POS_8998F4, 0x008998F4);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void __fastcall HudStringShow_LoadContextsAndEventPosition(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    if (!CSF_AT(void*, self, 0x8) ||
        !CSF_AT(void*, self, 0x4) ||
        !CSF_AT(void*, self, 0xC))
    {
        return;
    }

    HudStringShow_LoadTextureDimensions(
        CSF_AT(void*, self, 0x8), nullptr, properties);
    HudStringShow_LoadTextureDimensions(
        CSF_AT(void*, self, 0x4), nullptr, properties);
    S_004f41e0_LoadGlobalContext(
        CSF_AT(S_004f41e0*, self, 0xC), nullptr, properties);

    auto* eventContext = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(g_pszKey_CONTEXTO_EVENTOS));

    if (!eventContext)
    {
        SetSubtitlePanelPosition(
            CSF_AT(void*, self, 0x8), nullptr, 0.0f, 0.5f);
        return;
    }

    auto* position = reinterpret_cast<S_00692b80*>(
        LookupNamedElement(eventContext, nullptr, g_pszKey_POS_8998F4));
    auto* xComponent = S_00692b80_GetComponent(position, nullptr, 0);
    auto* yComponent = S_00692b80_GetComponent(position, nullptr, 1u);

    const int pixelY = S_00692b80_ParseIntegerComponent(yComponent, nullptr);
    const int pixelX = S_00692b80_ParseIntegerComponent(xComponent, nullptr);
    const float y = static_cast<float>(pixelY) / static_cast<float>(g_ScreenHeight);
    const float x = static_cast<float>(pixelX) / static_cast<float>(g_ScreenWidth);

    SetSubtitlePanelPosition(CSF_AT(void*, self, 0x8), nullptr, x, y);
}

HOOK(0x004F4AD0, HudStringShow_LoadContextsAndEventPosition, "auto");
