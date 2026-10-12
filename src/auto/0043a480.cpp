#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Hud_CreateTextureWidget, 0x0042D750, (S_00692b80* properties, int layer));
CSF_FUNC(void, __cdecl, Hud_ReadRectFromProperties, 0x0042DF00, (S_00692b80* a1, int* a2));
CSF_FUNC(uint64_t, __cdecl, ScaleHudSizePair, 0x0042F3B0, (int* sizePair));
CSF_FUNC(char, __fastcall, Hud_LoadScaleVertexBuffer, 0x00439EE0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, S_00439f80_EnsureCapacity, 0x0043A160, (void* self, void* edx, unsigned int a2));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatProperty, 0x00693940, (std::uint8_t* self, void* edx, char* name));

CSF_GLOBAL(float, g_NegativeHalfScale, 0x0082D4B8);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_FacingAngleOffset, 0x0082E044);
CSF_GLOBAL(float, g_MaximumScaleAngle, 0x00830D0C);
CSF_GLOBAL(const char*, g_pszKey_DEGREE, 0x00899668);
CSF_GLOBAL(const char*, g_pszKey_NUM_ESCALAS, 0x0089966C);
CSF_GLOBAL(const char*, g_pszKey_ALPHA_SOLID, 0x0089967C);

static char __fastcall Hud_InitializeAngleScale(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    if (!properties)
        return 0;

    auto* node = reinterpret_cast<unsigned char*>(properties);
    auto* component = reinterpret_cast<S_00692b80*>(
        GetComponent(node, nullptr, 0));
    if (!component)
        return 0;

    double angle = ReadFloatProperty(
        reinterpret_cast<std::uint8_t*>(component), nullptr,
        const_cast<char*>(g_pszKey_DEGREE));
    CSF_AT(float, self, 0x10) = static_cast<float>(angle);
    if (angle > g_MaximumScaleAngle)
        angle = g_MaximumScaleAngle;
    CSF_AT(float, self, 0x10) = static_cast<float>(angle);

    component = reinterpret_cast<S_00692b80*>(GetComponent(node, nullptr, 1));
    if (!component)
        return 0;

    alignas(8) std::uint8_t rectBytes[0x14];
    CSF_AT(int, rectBytes, 0x00) = 0;
    CSF_AT(int, rectBytes, 0x04) = 0;
    CSF_AT(int, rectBytes, 0x08) = 0;
    CSF_AT(int, rectBytes, 0x0C) = 0;
    Hud_ReadRectFromProperties(component, reinterpret_cast<int*>(rectBytes));

    CSF_AT(int, self, 0x00) = CSF_AT(int, rectBytes, 0x00);
    CSF_AT(int, self, 0x04) = CSF_AT(int, rectBytes, 0x04);
    ScaleHudSizePair(reinterpret_cast<int*>(self));

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, rectBytes, 0x08) -
        CSF_AT(std::uint32_t, rectBytes, 0x00);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, rectBytes, 0x0C) -
        CSF_AT(std::uint32_t, rectBytes, 0x04);
    ScaleHudSizePair(reinterpret_cast<int*>(
        static_cast<std::uint8_t*>(self) + 0x08));

    component = reinterpret_cast<S_00692b80*>(GetComponent(node, nullptr, 2));
    int textureWidget = Hud_CreateTextureWidget(component, 0);
    CSF_AT(int, self, 0x44) = textureWidget;
    if (!textureWidget)
        return 0;

    void* widget = reinterpret_cast<void*>(textureWidget);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, widget, 0)[0x70 / 4])(widget, nullptr, 1);

    component = reinterpret_cast<S_00692b80*>(GetComponent(node, nullptr, 3));
    if (!component)
        return 0;

    CSF_AT(int, rectBytes, 0x10) = ReadIntegerProperty(
        reinterpret_cast<std::uint8_t*>(component), nullptr,
        const_cast<char*>(g_pszKey_NUM_ESCALAS));
    if (CSF_AT(int, rectBytes, 0x10) < 2)
        CSF_AT(int, rectBytes, 0x10) = 2;

    component = reinterpret_cast<S_00692b80*>(GetComponent(node, nullptr, 4));
    if (!Hud_LoadScaleVertexBuffer(self, nullptr, component))
        return 0;

    int width = CSF_AT(int, self, 0x08);
    double scaleFraction =
        static_cast<double>(g_TransitionOne) / CSF_AT(int, rectBytes, 0x10);
    CSF_AT(int, self, 0x2C) = CSF_AT(int, self, 0x0C);
    int stepWidth = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(width) / CSF_AT(float, self, 0x10) *
        g_FacingAngleOffset * scaleFraction));
    CSF_AT(int, self, 0x28) = stepWidth;
    int minimumWidth = static_cast<int>(
        static_cast<std::uint32_t>(stepWidth) * 2);
    CSF_AT(float, self, 0x14) =
        static_cast<float>(scaleFraction * g_FacingAngleOffset);
    if (width < minimumWidth)
        width = minimumWidth;
    CSF_AT(int, self, 0x08) = width;

    component = reinterpret_cast<S_00692b80*>(GetComponent(node, nullptr, 5));
    if (!component)
        return 0;

    double solidFraction = ReadFloatProperty(
        reinterpret_cast<std::uint8_t*>(component), nullptr,
        const_cast<char*>(g_pszKey_ALPHA_SOLID));
    if (solidFraction < g_fZero)
        solidFraction = g_fZero;
    else if (solidFraction > g_TransitionOne)
        solidFraction = g_TransitionOne;

    width = CSF_AT(int, self, 0x08);
    int solidWidth = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(width) * solidFraction));
    int remainingWidth = static_cast<int>(
        static_cast<std::uint32_t>(width) -
        static_cast<std::uint32_t>(solidWidth));
    int offset = static_cast<int>(static_cast<std::int64_t>(
        static_cast<double>(remainingWidth) * g_NegativeHalfScale));

    std::uint32_t left = CSF_AT(std::uint32_t, self, 0x00) -
        static_cast<std::uint32_t>(offset);
    std::uint32_t top = CSF_AT(std::uint32_t, self, 0x04);
    CSF_AT(std::uint32_t, self, 0xDC) = left;
    CSF_AT(std::uint32_t, self, 0xE4) =
        left + static_cast<std::uint32_t>(solidWidth);
    std::uint32_t bottom = top + CSF_AT(std::uint32_t, self, 0x0C);
    CSF_AT(std::uint32_t, self, 0xE0) = top;
    int capacity = width / CSF_AT(int, self, 0x28);
    CSF_AT(std::uint32_t, self, 0xE8) = bottom;
    CSF_AT(int, self, 0x3C) = 0;
    S_00439f80_EnsureCapacity(
        static_cast<std::uint8_t*>(self) + 0x34, nullptr,
        static_cast<unsigned int>(capacity) + 2);
    return 1;
}

HOOK(0x0043A480, Hud_InitializeAngleScale, "auto");
