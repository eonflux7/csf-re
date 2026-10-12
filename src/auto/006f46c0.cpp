#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(int, __fastcall, Bounds_GetExtents, 0x006AB4A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5, uint32_t* a6, uint32_t* a7));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(void, __fastcall, GfxPrtStdParticles_ReadEmitterProperties, 0x006F35C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, GfxParticleRender_InitializeEmitterProperties, 0x007037F0, (uint32_t* emitter, const void* parameters));

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

// The packet also lists this call target as a vtable reference.
[[maybe_unused]] static const auto g_BoundsInitializerVtable = CSF_VTBL(0x006ABB80);

static float* __fastcall GfxPrtStdParticles_ConfigureEmitter(void* self, void* /*edx*/)
{
    const int emitter = RpPrtStdAtomicGetEmitter(CSF_AT(int, self, 0x24));

    alignas(8) uint8_t emitterProperties[0x28];
    GfxPrtStdParticles_ReadEmitterProperties(emitterProperties, nullptr, emitter);

    alignas(8) uint8_t parameters[0x24] = {};
    void* definition = CSF_AT(void*, self, 0x18);
    CSF_AT(uint32_t, parameters, 0x00) = CSF_AT(uint32_t, self, 0x38);
    CSF_AT(uint8_t, parameters, 0x04) = CSF_AT(uint8_t, definition, 0x61);
    CSF_AT(uint8_t, parameters, 0x05) = CSF_AT(uint8_t, definition, 0x60);
    CSF_AT(uint32_t, parameters, 0x06) = CSF_AT(uint32_t, definition, 0x4F);
    CSF_AT(uint32_t, parameters, 0x0A) = CSF_AT(uint32_t, definition, 0x53);
    CSF_AT(uint8_t, parameters, 0x0E) = CSF_AT(uint8_t, definition, 0x58);
    const uint32_t flags = CSF_AT(uint32_t, self, 0x14);
    CSF_AT(uint8_t, parameters, 0x0F) = CSF_AT(uint8_t, definition, 0x5C);
    CSF_AT(bool, parameters, 0x10) = (flags & 2) == 2;

    GfxParticleRender_InitializeEmitterProperties(
        reinterpret_cast<uint32_t*>(emitter), parameters);

    void* atomic = CSF_AT(void*, CSF_AT(void*, emitterProperties, 0x10), 0x18);
    CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0xAC) = 1;

    if (CSF_AT(uint8_t, CSF_AT(void*, self, 0x38), 0x1C))
    {
        CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0xA4) = 5;
        CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0xA8) = 2;
    }
    else
    {
        CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0xA4) = 5;
        CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0xA8) = 6;
    }

    CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0x40) |= 0x10000000u;

    uint8_t* textureCoordinates =
        static_cast<uint8_t*>(CSF_AT(void*, atomic, g_PTankPluginOffset)) + 0xD8;
    CSF_AT(float, parameters, 0x14) = 0.0f;
    CSF_AT(float, textureCoordinates, 0x00) = CSF_AT(float, parameters, 0x14);
    CSF_AT(float, parameters, 0x18) = 0.0f;
    CSF_AT(float, textureCoordinates, 0x04) = CSF_AT(float, parameters, 0x18);
    CSF_AT(float, parameters, 0x1C) = 1.0f;
    CSF_AT(float, textureCoordinates, 0x08) = CSF_AT(float, parameters, 0x1C);
    CSF_AT(float, parameters, 0x20) = 1.0f;
    CSF_AT(float, textureCoordinates, 0x0C) = CSF_AT(float, parameters, 0x20);

    CSF_AT(uint32_t, CSF_AT(void*, atomic, g_PTankPluginOffset), 0x40) |= 0x80000u;

    const int texture = CSF_AT(int, CSF_AT(void*, self, 0x38), 0x28);
    void* geometry = CSF_AT(void*, atomic, 0x18);
    void* materials = CSF_AT(void*, geometry, 0x20);
    RpMaterialSetTexture(CSF_AT(uint32_t*, materials, 0x00), texture);

    float xFirst = 0.0f, xSecond = 0.0f;
    float yFirst = 0.0f, ySecond = 0.0f;
    float zFirst = 0.0f, zSecond = 0.0f;
    Bounds_GetExtents(
        static_cast<uint8_t*>(CSF_AT(void*, self, 0x38)) + 0x2C, nullptr,
        reinterpret_cast<uint32_t*>(&xFirst), reinterpret_cast<uint32_t*>(&xSecond),
        reinterpret_cast<uint32_t*>(&yFirst), reinterpret_cast<uint32_t*>(&ySecond),
        reinterpret_cast<uint32_t*>(&zFirst), reinterpret_cast<uint32_t*>(&zSecond));

    return S_006abb80_InitializeBounds(
        reinterpret_cast<S_006abb80*>(static_cast<uint8_t*>(self) + 0x44),
        nullptr, xFirst, xSecond, yFirst, ySecond, zFirst, zSecond);
}

HOOK(0x006F46C0, GfxPrtStdParticles_ConfigureEmitter, "auto");
