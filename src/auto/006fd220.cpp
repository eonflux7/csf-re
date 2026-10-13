#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_StorePropertyValueAtOffset, 0x006B88E0, (int offset, int value));

CSF_GLOBAL(float, g_ColorWhite,   0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);
CSF_GLOBAL(float, g_ColorWhite_A, 0x008A7D9C);
CSF_GLOBAL(float, g_ColorBlack,   0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __fastcall CLightObject_InitializeProperties(
    CLightObject* self, void* /*edx*/, int owner)
{
    const int light = CSF_AT(int, self, 0x0C);
    CSF_AT(int, self, 0x08) = owner;
    GfxObjectProps_StorePropertyValueAtOffset(
        light, static_cast<int>(reinterpret_cast<std::uintptr_t>(self)));

    void* lightObject = CSF_AT(void*, self, 0x0C);
    CSF_AT(std::uint8_t, self, 0x80) = 0;
    CSF_AT(std::uint8_t, self, 0x3C) = 1;
    CSF_AT(std::uint8_t, self, 0x3D) = 1;

    const std::uint32_t red   = CSF_AT(std::uint32_t, lightObject, 0x18);
    const std::uint32_t green = CSF_AT(std::uint32_t, lightObject, 0x1C);
    const std::uint32_t blue  = CSF_AT(std::uint32_t, lightObject, 0x20);
    const std::uint32_t alpha = CSF_AT(std::uint32_t, lightObject, 0x24);
    CSF_AT(std::uint32_t, self, 0x40) = red;
    CSF_AT(std::uint32_t, self, 0x44) = green;
    CSF_AT(std::uint32_t, self, 0x48) = blue;
    CSF_AT(std::uint32_t, self, 0x4C) = alpha;

    CSF_AT(float, self, 0x70) = g_ColorBlack;
    CSF_AT(float, self, 0x74) = g_ColorBlack_G;
    CSF_AT(float, self, 0x78) = g_ColorBlack_B;
    CSF_AT(float, self, 0x7C) = g_ColorBlack_A;

    CSF_AT(float, self, 0x50) = g_ColorWhite;
    CSF_AT(float, self, 0x54) = g_ColorWhite_G;
    CSF_AT(float, self, 0x58) = g_ColorWhite_B;
    CSF_AT(float, self, 0x5C) = g_ColorWhite_A;

    CSF_AT(float, self, 0x60) = g_ColorBlack;
    CSF_AT(float, self, 0x64) = g_ColorBlack_G;
    CSF_AT(float, self, 0x68) = g_ColorBlack_B;
    CSF_AT(float, self, 0x6C) = g_ColorBlack_A;

    CSF_AT(std::uint8_t, self, 0x81) = 1;
    CSF_AT(std::uint8_t, self, 0x34) = 0;
    CSF_AT(int, self, 0x38) = 0;
    CSF_AT(int, self, 0x30) = -1;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(
        reinterpret_cast<std::uint8_t*>(self) + 0x60));
}

HOOK(0x006FD220, CLightObject_InitializeProperties, "auto");
