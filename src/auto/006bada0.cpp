#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetName, 0x005870B0, (int a1));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetInt, 0x0058BE50, (int a1, int a2));
CSF_FUNC(void*, __fastcall, GfxObjectProps_FindMaterialByName, 0x006BA370, (void* self, void* edx, void* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, g_MaterialPluginOffset, 0x008BB474);

static bool GfxObjectProps_MatchesPropertyName(
    const char* name, const char* expected)
{
    while (*name == *expected)
    {
        if (*expected == '\0')
            return true;
        ++name;
        ++expected;
    }
    return false;
}

static char __cdecl GfxObjectProps_ReadUserDataProperty(
    void* properties, int userData, int /*a3*/, int /*a4*/)
{
    const char* name =
        reinterpret_cast<const char*>(RpUserDataArrayGetName(userData));

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x00860520, "bLuzVolumetrica")))
    {
        CSF_AT(uint8_t, properties, 0x11) =
            RpUserDataArrayGetInt(userData, 0) != 0;
        return 1;
    }

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x00860510, "flagNoShadow")))
    {
        CSF_AT(uint8_t, properties, 0x0) =
            RpUserDataArrayGetInt(userData, 0) != 0;
        return 1;
    }

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x008603B4, "flagLightmap")))
    {
        CSF_AT(uint8_t, properties, 0x1) =
            RpUserDataArrayGetInt(userData, 0) != 0;
        return 1;
    }

    if (GfxObjectProps_MatchesPropertyName(
            name, CSF_STR(0x00860500, "strTipoMaterial")))
    {
        alignas(8) uint8_t materialNameBytes[0x1C];
        C_0082d410& materialName =
            *reinterpret_cast<C_0082d410*>(materialNameBytes);

        uint32_t* text =
            reinterpret_cast<uint32_t*>(RpUserDataArrayGetInt(userData, 0));
        C_0082d410_ConstructFromText(&materialName, nullptr, text);

        void* materialRegistry =
            CSF_AT(void*, g_pRwEngineInstance, g_MaterialPluginOffset);
        CSF_AT(void*, properties, 0x4) =
            GfxObjectProps_FindMaterialByName(
                materialRegistry, nullptr, &materialName);

        CMenuSaveGame_ResetStringStorage(&materialName, nullptr);
        return 1;
    }

    return 0;
}

HOOK(0x006BADA0, GfxObjectProps_ReadUserDataProperty, "auto");
