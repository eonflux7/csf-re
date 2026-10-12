#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPrtStdAtomicGetEmitter, 0x0057C290, (int a1));
CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());
CSF_FUNC(uint32_t*, __cdecl, RpMaterialSetTexture, 0x00591560, (uint32_t* a1, int a2));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80, (S_006abb80* self, void* , float xFirst, float xSecond, float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(void*, __fastcall, GfxSkin_RemoveTaggedEntry, 0x006C46B0, (void* self, void* , uint32_t handle));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(void, __fastcall, PrtStdParticles_GetEmitterProperties, 0x006F35C0, (void* self, void* edx, int a2));

CSF_GLOBAL(std::int32_t, g_PTankPluginOffset, 0x008B09EC);

static uint32_t* __fastcall PrtStdParticles_ApplyEmitterSettings(
    void* self, void* /*edx*/)
{
    alignas(8) uint8_t propertiesBytes[0x48];
    int emitter = RpPrtStdAtomicGetEmitter(CSF_AT(int, self, 0x24));
    PrtStdParticles_GetEmitterProperties(propertiesBytes, nullptr, emitter);

    uint32_t* standard = CSF_AT(uint32_t*, propertiesBytes, 0);
    std::memset(standard, 0, 0xAC);

    standard[1] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x08);

    uint32_t* source = reinterpret_cast<uint32_t*>(
        CSF_AT(uint8_t*, self, 0x30) + 0xD8);
    standard[7] = source[0];
    standard[8] = source[1];
    standard[9] = source[2];

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0xF4);
    standard[10] = source[0];
    standard[11] = source[1];
    standard[12] = source[2];

    standard[17] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x0C);
    standard[18] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x88);
    standard[13] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x10);
    standard[14] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x8C);
    standard[21] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x18);
    standard[22] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x90);

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x28);
    standard[23] = source[0];
    standard[24] = source[1];
    standard[25] = source[2];

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x98);
    standard[26] = source[0];
    standard[27] = source[1];
    standard[28] = source[2];

    standard[19] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x24);
    standard[20] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x94);

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x64);
    standard[4] = source[0];
    standard[5] = source[1];
    standard[6] = source[2];

    uint32_t* rotation = CSF_AT(uint32_t*, propertiesBytes, 0x08);
    if (rotation)
    {
        rotation[0] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x1C);
        rotation[1] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xCC);
        rotation[2] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0x20);
        rotation[3] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xD0);
    }

    void* settings = CSF_AT(void*, self, 0x30);
    uint32_t* size = CSF_AT(uint32_t*, propertiesBytes, 0x0C);
    size[0] = CSF_AT(uint32_t, settings, 0x34);
    size[1] = CSF_AT(uint32_t, settings, 0x38);
    settings = CSF_AT(void*, self, 0x30);
    size[4] = CSF_AT(uint32_t, settings, 0x3C);
    size[5] = CSF_AT(uint32_t, settings, 0x40);
    settings = CSF_AT(void*, self, 0x30);
    size[2] = CSF_AT(uint32_t, settings, 0xA4);
    size[3] = CSF_AT(uint32_t, settings, 0xA8);
    settings = CSF_AT(void*, self, 0x30);
    size[6] = CSF_AT(uint32_t, settings, 0xAC);
    size[7] = CSF_AT(uint32_t, settings, 0xB0);

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x44);
    uint32_t* color = CSF_AT(uint32_t*, propertiesBytes, 0x04);
    color[0] = source[0];
    color[1] = source[1];
    color[2] = source[2];
    color[3] = source[3];

    source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x54);
    color[8] = source[0];
    color[9] = source[1];
    color[10] = source[2];
    color[11] = source[3];
    color[7] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xC0);
    color[15] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xC4);

    void* tank = CSF_AT(void*, CSF_AT(void*, propertiesBytes, 0x10), 0x18);
    CSF_AT(uint32_t, CSF_AT(void*, tank, g_PTankPluginOffset), 0xAC) = 1;
    bool normalBlending = CSF_AT(uint8_t, CSF_AT(void*, self, 0x30), 0x70) == 0;
    CSF_AT(uint32_t, CSF_AT(void*, tank, g_PTankPluginOffset), 0xA4) = 5;

    void* tankData = CSF_AT(void*, tank, g_PTankPluginOffset);
    if (normalBlending)
    {
        CSF_AT(uint32_t, tankData, 0xA8) = 6;
        CSF_AT(uint32_t, CSF_AT(void*, tank, g_PTankPluginOffset), 0x40) |= 0x10000000u;
        void* owner = CSF_AT(void*, self, 0x1C);
        void* skin = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[28 / 4])(owner, nullptr);
        GfxSkin_RemoveTaggedEntry(skin, nullptr, 15);
    }
    else
    {
        CSF_AT(uint32_t, tankData, 0xA8) = 2;
        CSF_AT(uint32_t, CSF_AT(void*, tank, g_PTankPluginOffset), 0x40) |= 0x10000000u;
        void* owner = CSF_AT(void*, self, 0x1C);
        void* skin = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[28 / 4])(owner, nullptr);
        GfxSkin_SetKeyedValue(skin, nullptr, 15, 0);
    }

    settings = CSF_AT(void*, self, 0x30);
    uint32_t constantRed = CSF_AT(uint32_t, settings, 0x74);
    float constantGreen = CSF_AT(float, settings, 0x78);
    float constantBlue = CSF_AT(float, settings, 0x7C);
    float constantAlpha = CSF_AT(float, settings, 0x80);
    tankData = CSF_AT(void*, tank, g_PTankPluginOffset);
    CSF_AT(uint32_t, tankData, 0xD8) = constantRed;
    CSF_AT(float, tankData, 0xDC) = constantGreen;
    CSF_AT(float, tankData, 0xE0) = constantBlue;
    CSF_AT(float, tankData, 0xE4) = constantAlpha;
    CSF_AT(uint32_t, CSF_AT(void*, tank, g_PTankPluginOffset), 0x40) |= 0x80000u;

    standard[41] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0);
    RpMaterialSetTexture(
        CSF_AT(uint32_t*, CSF_AT(void*, CSF_AT(void*, tank, 0x18), 0x20), 0),
        CSF_AT(int, CSF_AT(void*, self, 0x30), 0));

    uint32_t* pointList = CSF_AT(uint32_t*, propertiesBytes, 0x24);
    pointList[0] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xE4);
    pointList[1] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xE8);
    pointList[2] = CSF_AT(uint32_t, CSF_AT(void*, self, 0x30), 0xEC);

    uint32_t* matrix = CSF_AT(uint32_t*, propertiesBytes, 0x14);
    if (matrix)
    {
        source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x10C);
        matrix[16] = source[0];
        matrix[17] = source[1];
        matrix[18] = source[2];
        source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x124);
        matrix[19] = source[0];
        matrix[20] = source[1];
        matrix[21] = source[2];
        source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x118);
        matrix[22] = source[0];
        matrix[23] = source[1];
        matrix[24] = source[2];
        source = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x30) + 0x130);
        matrix[25] = source[0];
        matrix[26] = source[1];
        matrix[27] = source[2];
        matrix[28] = 1;
        CSF_AT(float, matrix, 0x28) = 1.0f;
        CSF_AT(float, matrix, 0x14) = 1.0f;
        CSF_AT(float, matrix, 0) = 1.0f;
        matrix[4] = 0;
        matrix[2] = 0;
        matrix[1] = 0;
        matrix[9] = 0;
        matrix[8] = 0;
        matrix[6] = 0;
        matrix[14] = 0;
        matrix[13] = 0;
        matrix[12] = 0;
        matrix[3] |= 0x20003u;
    }

    standard[0] = RtRandom();
    settings = CSF_AT(void*, self, 0x30);
    float xSecond = CSF_AT(float, settings, 0x13C) + CSF_AT(float, standard, 0x1C);
    float ySecond = CSF_AT(float, settings, 0x140) + CSF_AT(float, standard, 0x20);
    float zSecond = CSF_AT(float, settings, 0x144) + CSF_AT(float, standard, 0x24);
    float xFirst = CSF_AT(float, settings, 0x148) + CSF_AT(float, standard, 0x1C);
    float yFirst = CSF_AT(float, settings, 0x14C) + CSF_AT(float, standard, 0x20);
    float zFirst = CSF_AT(float, settings, 0x150) + CSF_AT(float, standard, 0x24);

    // The packet also lists the bounds callee address as a vtable reference.
    (void)CSF_VTBL(0x006ABB80);
    S_006abb80_InitializeBounds(
        reinterpret_cast<S_006abb80*>(static_cast<uint8_t*>(self) + 0x44),
        nullptr, xFirst, xSecond, yFirst, ySecond, zFirst, zSecond);

    uint32_t* userData = CSF_AT(uint32_t*, propertiesBytes, 0x1C);
    userData[0] = CSF_AT(uint32_t, self, 0x30);
    return userData;
}

HOOK(0x006F4870, PrtStdParticles_ApplyEmitterSettings, "auto");
