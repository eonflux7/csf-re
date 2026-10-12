#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszKey_TEXTURAS_STRINGSHOW, 0x008998D8);

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

static int __fastcall HudStringShow_LoadTextureDimensions(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    (void)CSF_VTBL(0x00692B80);

    auto* textureProperties = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties), nullptr,
        const_cast<char*>(g_pszKey_TEXTURAS_STRINGSHOW));
    if (!textureProperties)
        return 0;

    const std::uint32_t childCount =
        MenuExtras_GetChildCount(textureProperties, nullptr);
    if (childCount != 1)
        return static_cast<int>(childCount);

    alignas(8) std::uint8_t rectangleBytes[0x10];
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);
    rectangle[0] = 0;
    rectangle[1] = 0;
    rectangle[2] = 0;
    rectangle[3] = 0;

    auto* component = reinterpret_cast<S_00692b80*>(
        GetComponent(textureProperties, nullptr, 0));
    Hud_ReadRectFromProperties(component, rectangle);

    CSF_AT(int, self, 0x14) = rectangle[2] - rectangle[0];
    CSF_AT(int, self, 0x18) = rectangle[3] - rectangle[1];
    ScaleHudSizePair(&CSF_AT(int, self, 0x14));

    alignas(8) std::uint8_t screenSizeBytes[0x8];
    float* screenSize = reinterpret_cast<float*>(screenSizeBytes);
    screenSize[0] = 0.0f;
    screenSize[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(screenSize);

    const std::int64_t scaledLeft = static_cast<std::int64_t>(
        static_cast<double>(rectangle[0]) * screenSize[0]);
    const int result = static_cast<int>(scaledLeft);
    CSF_AT(int, self, 0x1C) = result;
    return result;
}

HOOK(0x004F1DC0, HudStringShow_LoadTextureDimensions, "auto");
