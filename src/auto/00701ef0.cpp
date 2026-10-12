#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vector3ExactEquals, 0x004069B0, (float* self, void* , float* other));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(std::uint32_t, g_ParticleReadParam, 0x0086103C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(const char*, g_pszKey_MAX_PARTICULAS, 0x008A7F7C);
CSF_GLOBAL(const char*, g_pszKey_AREA_EMISION, 0x008A7F80);
CSF_GLOBAL(const char*, g_pszKey_EMISSION_DURATION, 0x008A7F84);
CSF_GLOBAL(const char*, g_pszKey_PART_PER_EMIS, 0x008A7F88);
CSF_GLOBAL(const char*, g_pszKey_PART_PER_EMIS_BIAS, 0x008A7F8C);
CSF_GLOBAL(const char*, g_pszKey_GAP_EMIS, 0x008A7F90);
CSF_GLOBAL(const char*, g_pszKey_GAP_EMIS_BIAS, 0x008A7F94);
CSF_GLOBAL(const char*, g_pszKey_PART_LIFE, 0x008A7F98);
CSF_GLOBAL(const char*, g_pszKey_PART_LIFE_BIAS, 0x008A7F9C);
CSF_GLOBAL(const char*, g_pszKey_PART_INIT_VELO, 0x008A7FA0);
CSF_GLOBAL(const char*, g_pszKey_PART_INIT_VELO_BIAS, 0x008A7FA4);
CSF_GLOBAL(const char*, g_pszKey_PART_INIT_DIR, 0x008A7FA8);
CSF_GLOBAL(const char*, g_pszKey_PART_INIT_DIR_BIAS, 0x008A7FAC);
CSF_GLOBAL(const char*, g_pszKey_FORCE_EMIS, 0x008A7FB0);
CSF_GLOBAL(const char*, g_pszKey_PART_2D_ROTATION_START, 0x008A7FB4);
CSF_GLOBAL(const char*, g_pszKey_PART_2D_ROTATION_START_BIAS, 0x008A7FB8);
CSF_GLOBAL(const char*, g_pszKey_PART_2D_ROTATION_END, 0x008A7FBC);
CSF_GLOBAL(const char*, g_pszKey_PART_2D_ROTATION_END_BIAS, 0x008A7FC0);
CSF_GLOBAL(const char*, g_pszKey_PART_START_COLOR, 0x008A7FC4);
CSF_GLOBAL(const char*, g_pszKey_PART_START_COLOR_BIAS, 0x008A7FC8);
CSF_GLOBAL(const char*, g_pszKey_PART_END_COLOR, 0x008A7FCC);
CSF_GLOBAL(const char*, g_pszKey_PART_END_COLOR_BIAS, 0x008A7FD0);
CSF_GLOBAL(const char*, g_pszKey_PART_START_SIZE, 0x008A7FE0);
CSF_GLOBAL(const char*, g_pszKey_PART_START_SIZE_BIAS, 0x008A7FE4);
CSF_GLOBAL(const char*, g_pszKey_PART_END_SIZE, 0x008A7FE8);
CSF_GLOBAL(const char*, g_pszKey_PART_END_SIZE_BIAS, 0x008A7FEC);
CSF_GLOBAL(const char*, g_pszKey_PART_UV0, 0x008A7FF8);
CSF_GLOBAL(const char*, g_pszKey_PART_UV1, 0x008A7FFC);
CSF_GLOBAL(const char*, g_pszKey_TEXTURE_gfx_texture_loader, 0x008A8000);
CSF_GLOBAL(const char*, g_pszKey_ADITIVE, 0x008A8008);
CSF_GLOBAL(const char*, g_pszKey_BBOX, 0x008A800C);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static std::uint32_t ParticleSystemDefs_ComponentAddress(std::uint8_t* node)
{
    return reinterpret_cast<std::uint32_t>(node);
}

static std::uint8_t* ParticleSystemDefs_FindProperty(
    std::uint8_t* definition, const char* key)
{
    return FindProperty(definition, nullptr, const_cast<char*>(key));
}

static float ParticleSystemDefs_ReadFloat(std::uint8_t* property)
{
    return static_cast<float>(
        ReadComponentFloat(ParticleSystemDefs_ComponentAddress(property)));
}

static float ParticleSystemDefs_ReadVectorComponent(
    std::uint8_t* property, unsigned int index)
{
    return static_cast<float>(
        ReadComponentFloat(GetComponent(property, nullptr, index)));
}

static char __fastcall ParticleSystemDefs_ReadEmitterParameters(
    float* self, void* /*edx*/, char* resourceName, std::uint8_t* definition)
{
    auto* property = ParticleSystemDefs_FindProperty(definition, g_pszKey_MAX_PARTICULAS);
    CSF_AT(int, self, 0x08) =
        ParseIntegerComponent(ParticleSystemDefs_ComponentAddress(property));

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_AREA_EMISION);
    CSF_AT(float, self, 0xF4) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0xF8) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    CSF_AT(float, self, 0xFC) = ParticleSystemDefs_ReadVectorComponent(property, 2);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_PER_EMIS);
    CSF_AT(int, self, 0x0C) =
        ParseIntegerComponent(ParticleSystemDefs_ComponentAddress(property));
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_PER_EMIS_BIAS);
    if (property)
        CSF_AT(int, self, 0x88) =
            ParseIntegerComponent(ParticleSystemDefs_ComponentAddress(property));

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_EMISSION_DURATION);
    CSF_AT(float, self, 0x14) = ParticleSystemDefs_ReadFloat(property);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_GAP_EMIS);
    const double emissionGap =
        ReadComponentFloat(ParticleSystemDefs_ComponentAddress(property));
    CSF_AT(float, self, 0x10) = static_cast<float>(emissionGap);
    if (emissionGap < g_MaximumHeadTimeStep)
        CSF_AT(float, self, 0x10) = 0.029999999f;

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_GAP_EMIS_BIAS);
    if (property)
        CSF_AT(float, self, 0x8C) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_LIFE);
    CSF_AT(float, self, 0x18) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_LIFE_BIAS);
    if (property)
        CSF_AT(float, self, 0x90) = ParticleSystemDefs_ReadFloat(property);

    const float activeDuration = CSF_AT(float, self, 0x14) == g_fZero
        ? CSF_AT(float, self, 0x90) + CSF_AT(float, self, 0x18)
        : CSF_AT(float, self, 0x10) + CSF_AT(float, self, 0x14);
    const std::int32_t particlesPerEmission = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, self, 0x0C) + CSF_AT(std::uint32_t, self, 0x88));
    const double requiredParticles =
        activeDuration / CSF_AT(float, self, 0x10) * static_cast<double>(particlesPerEmission);
    if (static_cast<double>(CSF_AT(int, self, 0x08)) > requiredParticles)
    {
        const auto rounded = static_cast<std::int64_t>(
            (requiredParticles + g_MessageBoxAnimationScale) * g_flt_0082DC84);
        CSF_AT(std::uint32_t, self, 0x08) =
            10u * static_cast<std::uint32_t>(rounded);
    }

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_INIT_VELO);
    CSF_AT(float, self, 0x24) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_INIT_VELO_BIAS);
    if (property)
        CSF_AT(float, self, 0x94) = ParticleSystemDefs_ReadFloat(property);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_INIT_DIR);
    CSF_AT(float, self, 0x28) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x2C) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    CSF_AT(float, self, 0x30) = ParticleSystemDefs_ReadVectorComponent(property, 2);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_INIT_DIR_BIAS);
    if (property)
    {
        CSF_AT(float, self, 0x98) = ParticleSystemDefs_ReadVectorComponent(property, 0);
        CSF_AT(float, self, 0x9C) = ParticleSystemDefs_ReadVectorComponent(property, 1);
        CSF_AT(float, self, 0xA0) = ParticleSystemDefs_ReadVectorComponent(property, 2);
    }

    if (g_fZero != CSF_AT(float, self, 0x94) + CSF_AT(float, self, 0x24)
        && CSF_AT(float, self, 0x28) == g_ZeroVectorX
        && CSF_AT(float, self, 0x2C) == g_ZeroVectorY
        && CSF_AT(float, self, 0x30) == g_ZeroVectorZ
        && static_cast<std::uint8_t>(Vector3ExactEquals(
            &CSF_AT(float, self, 0x98), nullptr, &g_ZeroVectorX)))
    {
        CSF_AT(float, self, 0x28) = g_AxisY;
        CSF_AT(float, self, 0x2C) = g_AxisY_Y;
        CSF_AT(float, self, 0x30) = g_AxisY_Z;
    }

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_FORCE_EMIS);
    CSF_AT(float, self, 0x64) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x68) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    CSF_AT(float, self, 0x6C) = ParticleSystemDefs_ReadVectorComponent(property, 2);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_2D_ROTATION_START);
    CSF_AT(float, self, 0x1C) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_2D_ROTATION_START_BIAS);
    if (property)
        CSF_AT(float, self, 0xCC) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_2D_ROTATION_END);
    CSF_AT(float, self, 0x20) = ParticleSystemDefs_ReadFloat(property);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_2D_ROTATION_END_BIAS);
    if (property)
        CSF_AT(float, self, 0xD0) = ParticleSystemDefs_ReadFloat(property);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_START_COLOR);
    CSF_AT(float, self, 0x44) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x48) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    CSF_AT(float, self, 0x4C) = ParticleSystemDefs_ReadVectorComponent(property, 2);
    CSF_AT(float, self, 0x50) = ParticleSystemDefs_ReadVectorComponent(property, 3);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_END_COLOR);
    CSF_AT(float, self, 0x54) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x58) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    CSF_AT(float, self, 0x5C) = ParticleSystemDefs_ReadVectorComponent(property, 2);
    CSF_AT(float, self, 0x60) = ParticleSystemDefs_ReadVectorComponent(property, 3);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_START_COLOR_BIAS);
    if (property)
        CSF_AT(float, self, 0xC0) = ParticleSystemDefs_ReadVectorComponent(property, 3);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_END_COLOR_BIAS);
    if (property)
        CSF_AT(float, self, 0xC4) = ParticleSystemDefs_ReadVectorComponent(property, 3);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_START_SIZE);
    CSF_AT(float, self, 0x34) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x38) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_START_SIZE_BIAS);
    if (property)
    {
        CSF_AT(float, self, 0xA4) = ParticleSystemDefs_ReadVectorComponent(property, 0);
        CSF_AT(float, self, 0xA8) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    }

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_END_SIZE);
    CSF_AT(float, self, 0x3C) = ParticleSystemDefs_ReadVectorComponent(property, 0);
    CSF_AT(float, self, 0x40) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_END_SIZE_BIAS);
    if (property)
    {
        CSF_AT(float, self, 0xAC) = ParticleSystemDefs_ReadVectorComponent(property, 0);
        CSF_AT(float, self, 0xB0) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    }

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_UV0);
    if (property)
    {
        CSF_AT(float, self, 0x74) = ParticleSystemDefs_ReadVectorComponent(property, 0);
        CSF_AT(float, self, 0x78) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    }
    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_PART_UV1);
    if (property)
    {
        CSF_AT(float, self, 0x7C) = ParticleSystemDefs_ReadVectorComponent(property, 0);
        CSF_AT(float, self, 0x80) = ParticleSystemDefs_ReadVectorComponent(property, 1);
    }

    alignas(8) std::uint8_t sourceBytes[0x100];
    alignas(8) std::uint8_t destinationBytes[0x110];
    char* source = reinterpret_cast<char*>(sourceBytes);
    char* destination = reinterpret_cast<char*>(destinationBytes);

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_TEXTURE_gfx_texture_loader);
    CopyAttributeText(property, nullptr, source, 256);
    if (std::strlen(source) != 0)
    {
        ResourceName_Transform(
            resourceName, const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
            destination, 256);

        std::size_t length = std::strlen(destination);
        if (length < 0xFF)
        {
            _strncpy(destination + length, source, 256 - length);
            destination[255] = 0;
        }
        length = std::strlen(destination);
        if (length < 0xFF)
        {
            _strncpy(destination + length,
                reinterpret_cast<const char*>(&g_ParticleReadParam), 256 - length);
            destination[255] = 0;
        }

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
        void* fileSystem = reinterpret_cast<void*>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[16 / 4])(accessor, nullptr));
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, fileSystem, 0)[28 / 4])(fileSystem, nullptr, destination))
        {
            CSF_AT(int, self, 0x00) = GlobalInt_FindNameId(sourceBytes);
        }
    }

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_ADITIVE);
    if (property)
        CSF_AT(std::uint8_t, self, 0x70) =
            ParseIntegerComponent(ParticleSystemDefs_ComponentAddress(property)) != 0;

    property = ParticleSystemDefs_FindProperty(definition, g_pszKey_BBOX);
    if (property)
    {
        const float x0 = ParticleSystemDefs_ReadVectorComponent(property, 0);
        const float y0 = ParticleSystemDefs_ReadVectorComponent(property, 1);
        const float z0 = ParticleSystemDefs_ReadVectorComponent(property, 2);
        const float x1 = ParticleSystemDefs_ReadVectorComponent(property, 3);
        const float y1 = ParticleSystemDefs_ReadVectorComponent(property, 4);
        const float z1 = ParticleSystemDefs_ReadVectorComponent(property, 5);

        CSF_AT(float, self, 0x13C) = x0 >= x1 ? x1 : x0;
        CSF_AT(float, self, 0x140) = y0 < y1 ? y0 : y1;
        CSF_AT(float, self, 0x144) = z0 < z1 ? z0 : z1;
        CSF_AT(float, self, 0x148) = x0 < x1 ? x1 : x0;
        CSF_AT(float, self, 0x14C) = y0 < y1 ? y1 : y0;
        CSF_AT(float, self, 0x150) = z0 < z1 ? z1 : z0;
    }

    CSF_AT(float, self, 0xD4) = 0.0f;
    CSF_AT(float, self, 0x100) = 0.0f;
    return 1;
}

HOOK(0x00701EF0, ParticleSystemDefs_ReadEmitterParameters, "auto");
