#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicCreate, 0x00582400, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);
CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);

static void* __cdecl GfxPTankCreate_GetPluginData(uint32_t atomic)
{
    return CSF_AT(void*, reinterpret_cast<void*>(atomic), g_PTankPluginOffset);
}

static char __fastcall C_00862110_CreateParticleAtomic(
    C_00862110* self, void* /*edx*/, int definitionAddress)
{
    void* definition = reinterpret_cast<void*>(definitionAddress);
    self->field_0030 = CSF_AT(uint32_t, definition, 0x20);

    int creationFlags = 269090817;
    if ((self->field_0030 & 0x30) != 0)
        creationFlags = 269123585;

    const int atomic = RpPTankAtomicCreate(1, creationFlags, 0);
    self->field_0004 = atomic;
    if (!atomic)
        return 0;

    RpAtomicSetFrame(atomic, CSF_AT(int, self, 0x14));

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x800000;

    void* plugin = GfxPTankCreate_GetPluginData(self->field_0004);
    CSF_AT(uint32_t, plugin, 0x04) = CSF_AT(uint32_t, plugin, 0x00);

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xC4) =
        g_DefaultParamA_8BB42C;

    const uint32_t materialAtomic = self->field_0004;
    void* geometry = CSF_AT(void*, reinterpret_cast<void*>(materialAtomic), 0x18);
    void* materials = CSF_AT(void*, geometry, 0x20);
    uint32_t* material = CSF_AT(uint32_t*, materials, 0x00);
    if (material)
    {
        CSF_AT(uint32_t, material, 0x04) =
            CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(materialAtomic), 0xC4);
    }

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x2000;
    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xAC) = 1;

    if ((self->field_0030 & 8) != 0)
    {
        CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xA4) = 5;
        CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xA8) = 2;
    }
    else
    {
        CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xA4) = 5;
        CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0xA8) = 6;
    }

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x10000000;

    void* globalInt = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalInt, nullptr));
    if (((int(__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[0x10 / 4])(
            accessor, nullptr))
    {
        const int texture = GlobalInt_FindNameId(CSF_AT(uint8_t*, definition, 0x00));
        if (texture)
        {
            geometry = CSF_AT(void*, reinterpret_cast<void*>(self->field_0004), 0x18);
            materials = CSF_AT(void*, geometry, 0x20);
            RpMaterialSetTexture(CSF_AT(uint32_t*, materials, 0x00), texture);
        }
    }

    const uint32_t color2 = CSF_AT(uint32_t, definition, 0x10);
    const uint32_t color0 = CSF_AT(uint32_t, definition, 0x08);
    const uint32_t color1 = CSF_AT(uint32_t, definition, 0x0C);
    const uint32_t color3 = CSF_AT(uint32_t, definition, 0x14);
    plugin = GfxPTankCreate_GetPluginData(self->field_0004);
    CSF_AT(uint32_t, plugin, 0xD8) = color0;
    CSF_AT(uint32_t, plugin, 0xDC) = color1;
    CSF_AT(uint32_t, plugin, 0xE0) = color2;
    CSF_AT(uint32_t, plugin, 0xE4) = color3;

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x80000;

    const uint32_t sizeX = CSF_AT(uint32_t, definition, 0x18);
    const uint32_t sizeY = CSF_AT(uint32_t, definition, 0x1C);
    plugin = GfxPTankCreate_GetPluginData(self->field_0004);
    CSF_AT(uint32_t, plugin, 0xB8) = sizeX;
    CSF_AT(uint32_t, plugin, 0xBC) = sizeY;

    CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x4000;

    const bool needsMatrix = (self->field_0030 & 0x30) != 0;
    self->field_0028 = sizeX;
    self->field_002c = sizeY;
    if (needsMatrix)
    {
        std::uint32_t* matrix = RwMatrixCreate();
        plugin = GfxPTankCreate_GetPluginData(self->field_0004);
        std::memcpy(static_cast<uint8_t*>(plugin) + 0xF8, matrix, 0x40);
        RwMatrixDestroy(matrix);
        CSF_AT(uint32_t, GfxPTankCreate_GetPluginData(self->field_0004), 0x40) |= 0x8000;
    }

    return 1;
}

HOOK(0x006EED20, C_00862110_CreateParticleAtomic, "auto");
