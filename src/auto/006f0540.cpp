#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));

static int __fastcall ParticlePTank_ConfigureAtomic(
    void* self, void* /*edx*/, void* particleDefinition, void* atomic)
{
    void** originalVtable = CSF_AT(void**, self, 0);
    CSF_AT(void*, self, 0x20) = particleDefinition;
    CSF_AT(void*, self, 0x24) = atomic;

    int definitionValue =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, particleDefinition, 0)[0x54 / 4])(
                particleDefinition, nullptr);
    ((void(__fastcall*)(void*, void*, int))originalVtable[0x50 / 4])(
        self, nullptr, definitionValue);

    void* currentAtomic = CSF_AT(void*, self, 0x24);
    if (!currentAtomic)
        return 0;

    void* materialDefinition =
        reinterpret_cast<std::uint8_t*>(particleDefinition) + 0x20;

    CSF_AT(std::uint32_t,
        CSF_AT(void*, currentAtomic, g_PTankPluginOffset), 0xAC) = 1;

    void* materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    std::uint8_t keyed = CSF_AT(std::uint8_t, materialSettings, 0x70);

    CSF_AT(std::uint32_t,
        CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset),
        0xA4) = 5;

    void* plugin =
        CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset);
    if (keyed)
    {
        CSF_AT(std::uint32_t, plugin, 0xA8) = 2;
        CSF_AT(std::uint32_t,
            CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset),
            0x40) |= 0x10000000u;

        void* skinProvider = CSF_AT(void*, self, 0x1C);
        void* skin =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, skinProvider, 0)[0x1C / 4])(
                    skinProvider, nullptr);
        GfxSkin_SetKeyedValue(skin, nullptr, 15, 0);
    }
    else
    {
        CSF_AT(std::uint32_t, plugin, 0xA8) = 6;
        CSF_AT(std::uint32_t,
            CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset),
            0x40) |= 0x10000000u;
    }

    materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    std::uint32_t component0 = CSF_AT(std::uint32_t, materialSettings, 0x74);

    materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    std::uint32_t component1 = CSF_AT(std::uint32_t, materialSettings, 0x78);

    materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    std::uint32_t component2 = CSF_AT(std::uint32_t, materialSettings, 0x7C);

    materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    std::uint32_t component3 = CSF_AT(std::uint32_t, materialSettings, 0x80);

    plugin = CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset);
    CSF_AT(std::uint32_t, plugin, 0xD8) = component0;
    CSF_AT(std::uint32_t, plugin, 0xDC) = component1;
    CSF_AT(std::uint32_t, plugin, 0xE0) = component2;
    CSF_AT(std::uint32_t, plugin, 0xE4) = component3;

    CSF_AT(std::uint32_t,
        CSF_AT(void*, CSF_AT(void*, self, 0x24), g_PTankPluginOffset),
        0x40) |= 0x80000u;

    materialSettings =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, materialDefinition, 0)[0x24 / 4])(
                materialDefinition, nullptr);
    int texture = CSF_AT(int, materialSettings, 0);
    void* geometry = CSF_AT(void*, CSF_AT(void*, self, 0x24), 0x18);
    void* materials = CSF_AT(void*, geometry, 0x20);
    uint32_t* material = CSF_AT(uint32_t*, materials, 0);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(
        RpMaterialSetTexture(material, texture)));
}

HOOK(0x006F0540, ParticlePTank_ConfigureAtomic, "auto");
