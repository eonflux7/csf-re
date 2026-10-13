#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudObjectives_CreateQuad, 0x00427860, (void* self, void* edx, void* a2, void* String1, uint32_t* a4));
CSF_FUNC(uint32_t*, __fastcall, HudObjectives_CreateTexture, 0x00427980, (void* self, void* edx, void* a2, void* String1, char* a4));
CSF_FUNC(int, __fastcall, HudObjectives_CreateFont, 0x00427AC0, (void* self, void* edx, void* a2, void* String1, char* a4));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_FindProperty, 0x006932B0, (S_00692b80* self, void* , char* name));
CSF_FUNC(int, __fastcall, S_00692b80_ReadIntegerProperty, 0x00693910, (S_00692b80* self, void* , char* keyword));
CSF_FUNC(double, __fastcall, S_00692b80_ReadFloatProperty, 0x00693940, (S_00692b80* self, void* , char* keyword));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, GuiEffect_Create, 0x00713B20, (int effectType));

CSF_GLOBAL(float, g_GravityScale, 0x0082E070);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

static uint32_t* __fastcall HudObjectives_ConstructBanner(
    uint32_t* self, void* /*edx*/, S_00692b80* properties)
{
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(float, self, 0x54) = 1.0f;

    CSF_AT(void*, self, 0x04) = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(uint8_t, self, 0x08) = 1;
    CSF_AT(uint8_t, self, 0x09) = 0;

    CSF_AT(int, self, 0x0C) = HudObjectives_CreateFont(
        self, nullptr, properties,
        const_cast<char*>(CSF_STR(0x0082FFB8, ".FONT_TITULO")),
        const_cast<char*>(CSF_STR(0x0082FFC8, "Menus/Mgame_01.fnt")));
    CSF_AT(int, self, 0x10) = HudObjectives_CreateFont(
        self, nullptr, properties,
        const_cast<char*>(CSF_STR(0x0082FF8C, ".FONT_TITULO_OBJETIVO")),
        const_cast<char*>(CSF_STR(0x0082FFA4, "Menus/Ingame_01.fnt")));
    CSF_AT(int, self, 0x14) = HudObjectives_CreateFont(
        self, nullptr, properties,
        const_cast<char*>(CSF_STR(0x0082FF78, ".FONT_DESCRIPCION")),
        const_cast<char*>(CSF_STR(0x0082FFA4, "Menus/Ingame_01.fnt")));
    CSF_AT(uint32_t*, self, 0x20) = HudObjectives_CreateTexture(
        self, nullptr, properties,
        const_cast<char*>(CSF_STR(0x0082FF48, ".TEXTURA_HIGHLIGHT")),
        const_cast<char*>(CSF_STR(0x0082FF5C, "Gfx/Textures/bannDest.png")));

    alignas(8) uint8_t colorBytes[0x10];
    auto* color = reinterpret_cast<uint32_t*>(colorBytes);
    color[0] = 1116602368;
    color[1] = 1106771968;
    color[2] = 1090519040;
    color[3] = 1123942400;
    CSF_AT(int, self, 0x18) = HudObjectives_CreateQuad(
        self, nullptr, properties,
        const_cast<char*>(CSF_STR(0x0082FF3C, ".COLOR_QUAD")), color);

    auto* effect = GuiEffect_Create(7);
    CSF_AT(uint32_t*, self, 0x1C) = effect;
    if (effect)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, effect, 0)[0x84 / 4])(
                effect, nullptr, 1, CSF_AT(void*, self, 0x04));
    }

    if (properties && S_00692b80_FindProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FF20, ".POSICION_VERTICAL_INICIAL"))))
        CSF_AT(int, self, 0x34) = S_00692b80_ReadIntegerProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FF20, ".POSICION_VERTICAL_INICIAL")));
    else
        CSF_AT(int, self, 0x34) = 47;

    if (properties && S_00692b80_FindProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FF00, ".ESPACIO_VERTICAL_ENTRE_TEXTOS"))))
        CSF_AT(int, self, 0x38) = S_00692b80_ReadIntegerProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FF00, ".ESPACIO_VERTICAL_ENTRE_TEXTOS")));
    else
        CSF_AT(int, self, 0x38) = 10;

    if (properties && S_00692b80_FindProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FEE4, ".LATENCIA_HIGHLIGHT_INICIAL"))))
        CSF_AT(float, self, 0x3C) = static_cast<float>(S_00692b80_ReadFloatProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FEE4, ".LATENCIA_HIGHLIGHT_INICIAL"))));
    else
        CSF_AT(float, self, 0x3C) = g_GravityScale;

    if (properties && S_00692b80_FindProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FECC, ".LATENCIA_FADE_FINAL"))))
        CSF_AT(float, self, 0x40) = static_cast<float>(S_00692b80_ReadFloatProperty(
            properties, nullptr,
            const_cast<char*>(CSF_STR(0x0082FECC, ".LATENCIA_FADE_FINAL"))));
    else
        CSF_AT(float, self, 0x40) = g_SoundActivityThreshold;

    CSF_AT(int, self, 0x00) = -1;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint8_t, self, 0x09) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2A2) = 1;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2A1) = 1;
    return self;
}

HOOK(0x00428140, HudObjectives_ConstructBanner, "auto");
