#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Hud_ConstructBanner, 0x00428140, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, Hud_CreateWidget, 0x0042D750, (void* a1, int a2));
CSF_FUNC(unsigned int, __cdecl, Hud_ScaleTextDimensions, 0x0042DE60, (float* a1));
CSF_FUNC(uint64_t, __cdecl, Hud_ScaleCoordinates, 0x0042F3B0, (int* a1));
CSF_FUNC(void, __fastcall, Hud_InitializeAlarmWidgets, 0x0042FF80, (void* self, void* edx, void* configuration));
CSF_FUNC(void, __fastcall, Hud_InitializeReticleBackground, 0x00430360, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, Hud_InitializePickupContext, 0x004303F0, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, Hud_InitializeDamageIndicator, 0x00430520, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, Hud_InitializeDisguiseWidgets, 0x00430640, (void* self, void* edx, void* arg0));
CSF_FUNC(void, __fastcall, Hud_InitializeRankWidgets, 0x00430860, (void* self, void* edx, void* arg0));
CSF_FUNC(void, __fastcall, Hud_InitializePlayerName, 0x00430A80, (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, Hud_InitializeSprites, 0x00431F80, (void* self, void* edx, void* configuration));
CSF_FUNC(void, __fastcall, Hud_InitializeCrosshairWidgets, 0x00432CB0, (void* self, void* edx, void* configuration));
CSF_FUNC(void, __fastcall, Hud_InitializeIconTextures, 0x00435D00, (void* self, void* edx, void* a5));
CSF_FUNC(void, __fastcall, Hud_InitializeFilters, 0x00436140, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, Hud_ConstructQuickLoadSave, 0x004383C0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, Hud_ConstructLinearRadar, 0x004399D0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Hud_InitializeLinearRadar, 0x0043A6A0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_TEXTURAS_ICONOS, 0x0089954C);
CSF_GLOBAL(const char*, g_pszKey_SPRITES, 0x00899550);
CSF_GLOBAL(const char*, g_pszKey_CROSSFIRES, 0x00899554);
CSF_GLOBAL(const char*, g_pszKey_FILTROS, 0x00899558);
CSF_GLOBAL(const char*, g_pszKey_DISFRACES, 0x0089955C);
CSF_GLOBAL(const char*, g_pszKey_GALONES, 0x00899560);
CSF_GLOBAL(const char*, g_pszKey_TEXTO_DISFRAZ, 0x00899564);
CSF_GLOBAL(const char*, g_pszKey_POS, 0x00899568);
CSF_GLOBAL(const char*, g_pszKey_FONDO_DATOS_MIRILLA, 0x00899574);
CSF_GLOBAL(const char*, g_pszKey_INDICADOR_DANYO, 0x008995D4);
CSF_GLOBAL(const char*, g_pszKey_VISTO_DE_LEJOS, 0x008995D8);
CSF_GLOBAL(const char*, g_pszKey_CONTEXTO_COGER_ITEM, 0x008995F0);
CSF_GLOBAL(const char*, g_pszKey_RADAR_LINEAL, 0x008995F4);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_PLAYER, 0x008995F8);
CSF_GLOBAL(const char*, g_pszKey_ALARMA, 0x0089960C);
CSF_GLOBAL(const char*, g_pszKey_BANNER, 0x00899618);
CSF_GLOBAL(const char*, g_pszKey_QUICK_LOADSAVE, 0x0089961C);

static const auto kMenuExtrasVtable = CSF_VTBL(0x00692B80);
static const auto kQuickLoadSaveVtable = CSF_VTBL(0x004383C0);

static std::uint8_t* __fastcall Hud_InitializeWidgets(
    void* self, void* /*edx*/, std::uint8_t* configuration)
{
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0xD0) = 0;
    CSF_AT(std::uint32_t, self, 0xE0) = 0;
    CSF_AT(std::uint32_t, self, 0x110) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    CSF_AT(std::uint32_t, self, 0xB4) = 0;

    if (CSF_AT(void*, self, 0xA0))
        Mem_FreeDebug(CSF_AT(void*, self, 0xA0));
    CSF_AT(std::uint32_t, self, 0xA0) = 0;
    CSF_AT(std::uint32_t, self, 0xA4) = 0;
    CSF_AT(std::uint32_t, self, 0xA8) = 0;

    void* radarStorage = Mem_Alloc(236);
    void* radar = radarStorage
        ? Hud_ConstructLinearRadar(radarStorage, nullptr) : nullptr;
    CSF_AT(void*, self, 0x24C) = radar;
    if (radar)
    {
        auto* property = FindProperty(
            configuration, nullptr, const_cast<char*>(g_pszKey_RADAR_LINEAL));
        Hud_InitializeLinearRadar(CSF_AT(void*, self, 0x24C), nullptr, property);
    }

    auto* disguiseText = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_TEXTO_DISFRAZ));
    if (disguiseText && !CSF_AT(void*, self, 0x208))
    {
        void* controls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        CSF_AT(void*, self, 0x208) =
            reinterpret_cast<void*(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, controls, 0)[84 / 4])(
                    controls, nullptr, CSF_AT(std::uint32_t, state, 0x8C));

        auto* positionProperty =
            LookupNamedElement(disguiseText, nullptr, g_pszKey_POS);
        if (positionProperty &&
            MenuExtras_GetChildCount(positionProperty, nullptr) == 2)
        {
            const std::uint32_t xComponent =
                GetComponent(positionProperty, nullptr, 0);
            const std::uint32_t yComponent =
                GetComponent(positionProperty, nullptr, 1u);
            const int y = ParseIntegerComponent(yComponent);
            int position[2];
            position[0] = ParseIntegerComponent(xComponent);
            position[1] = y;
            float dimensions[2] = {0.0f, 0.0f};
            Hud_ScaleCoordinates(position);
            Hud_ScaleTextDimensions(dimensions);

            void* widget = CSF_AT(void*, self, 0x208);
            reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, widget, 0)[80 / 4])(widget, nullptr, position);

            widget = CSF_AT(void*, self, 0x208);
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, widget, 0)[152 / 4])(widget, nullptr, dimensions);

            widget = CSF_AT(void*, self, 0x208);
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, widget, 0)[60 / 4])(
                    widget, nullptr,
                    CSF_STR(0x0083098C, "                              "));
        }
    }

    auto* property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_TEXTURAS_ICONOS));
    Hud_InitializeIconTextures(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_ALARMA));
    Hud_InitializeAlarmWidgets(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_SPRITES));
    Hud_InitializeSprites(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_CONTEXTO_COGER_ITEM));
    Hud_InitializePickupContext(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_INDICADOR_DANYO));
    Hud_InitializeDamageIndicator(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_VISTO_DE_LEJOS));
    if (!CSF_AT(void*, self, 0x104) && property &&
        MenuExtras_GetChildCount(property, nullptr))
    {
        auto* component =
            reinterpret_cast<void*>(GetComponent(property, nullptr, 0));
        CSF_AT(int, self, 0x104) = Hud_CreateWidget(component, 0);
    }

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_CROSSFIRES));
    Hud_InitializeCrosshairWidgets(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_FILTROS));
    Hud_InitializeFilters(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_DISFRACES));
    Hud_InitializeDisguiseWidgets(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_GALONES));
    Hud_InitializeRankWidgets(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_FONDO_DATOS_MIRILLA));
    Hud_InitializeReticleBackground(self, nullptr, property);

    property = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_NOMBRE_PLAYER));
    Hud_InitializePlayerName(self, nullptr, property);

    auto* bannerProperty = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_BANNER));
    if (!CSF_AT(void*, self, 0x2CC))
    {
        void* storage = Mem_Alloc(88);
        CSF_AT(void*, self, 0x2CC) = storage
            ? Hud_ConstructBanner(storage, nullptr, bannerProperty) : nullptr;
    }

    auto* result = FindProperty(
        configuration, nullptr, const_cast<char*>(g_pszKey_QUICK_LOADSAVE));
    if (result)
    {
        void* storage = Mem_Alloc(40);
        if (storage)
        {
            result = reinterpret_cast<std::uint8_t*>(
                Hud_ConstructQuickLoadSave(storage, nullptr, result));
            CSF_AT(void*, self, 0x2D0) = result;
        }
        else
        {
            CSF_AT(void*, self, 0x2D0) = nullptr;
            return nullptr;
        }
    }
    return result;
}

HOOK(0x00436420, Hud_InitializeWidgets, "auto");
