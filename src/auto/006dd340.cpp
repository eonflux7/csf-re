#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vector3ExactEquals, 0x004069B0, (float* self, void* , float* other));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, S_00692b10_InitializeFile, 0x00692B10, (S_00692b10* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(char*, __cdecl, GfxTextureLoader_AppendResourceName, 0x00694C30, (char* a1, int a2, char* Source, int a4, int a5, int a6, int a7, int a8, int a9, int a10));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardWithSkinVisitorCallback, 0x006C6E40, (int a1));
CSF_FUNC(int, __fastcall, GfxObject_AcquireIndexedResource, 0x006D4740, (S_006d4450* self, void* , int a2));
CSF_FUNC(int, __fastcall, GfxTextureLoader_FindModelIndex, 0x006D58E0, (S_006d4450* self, void* , char* String2, int));
CSF_FUNC(int, __cdecl, GfxTextureLoader_FindFilterType, 0x006DCEF0, (char* name));
CSF_FUNC(int, __cdecl, GfxTextureLoader_FindFacingType, 0x006DCF40, (char* name));
CSF_FUNC(int, __fastcall, S_006dcf90_ReleaseResource, 0x006DCF90, (S_006dcf90* self, void*));
CSF_FUNC(int, __fastcall, S_006dcf90_ResetDefaults, 0x006DD040, (S_006dcf90* self, void*));
CSF_FUNC(char, __fastcall, ParticleSystemDefs_ReadEmitterParameters, 0x00701EF0, (float* self, void* , char* resourceName, std::uint8_t* definition));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(std::uint32_t, g_ParticleReadParam, 0x0086103C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

CSF_GLOBAL(const char*, g_pszKey_VERSION_gfx_texture_loader, 0x008A7F78);
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
CSF_GLOBAL(const char*, g_pszKey_PART_END_COLOR, 0x008A7FCC);
CSF_GLOBAL(const char*, g_pszKey_PART_START_COLOR_ALPHA_BIAS, 0x008A7FD4);
CSF_GLOBAL(const char*, g_pszKey_PART_END_COLOR_ALPHA_BIAS, 0x008A7FD8);
CSF_GLOBAL(const char*, g_pszKey_PART_COLOR_BIAS, 0x008A7FDC);
CSF_GLOBAL(const char*, g_pszKey_PART_START_SIZE, 0x008A7FE0);
CSF_GLOBAL(const char*, g_pszKey_PART_START_SIZE_BIAS, 0x008A7FE4);
CSF_GLOBAL(const char*, g_pszKey_PART_END_SIZE, 0x008A7FE8);
CSF_GLOBAL(const char*, g_pszKey_PART_END_SIZE_BIAS, 0x008A7FEC);
CSF_GLOBAL(const char*, g_pszKey_PART_SIZE_SCALE_BIAS, 0x008A7FF0);
CSF_GLOBAL(const char*, g_pszKey_USE_REGULAR_SIZE_SCALE, 0x008A7FF4);
CSF_GLOBAL(const char*, g_pszKey_PART_UV0, 0x008A7FF8);
CSF_GLOBAL(const char*, g_pszKey_PART_UV1, 0x008A7FFC);
CSF_GLOBAL(const char*, g_pszKey_TEXTURE_gfx_texture_loader, 0x008A8000);
CSF_GLOBAL(const char*, g_pszKey_MODEL, 0x008A8004);
CSF_GLOBAL(const char*, g_pszKey_ADITIVE, 0x008A8008);
CSF_GLOBAL(const char*, g_pszKey_BBOX, 0x008A800C);
CSF_GLOBAL(const char*, g_pszKey_FRICCION, 0x008A8010);
CSF_GLOBAL(const char*, g_pszKey_AREA_TYPE, 0x008A8014);
CSF_GLOBAL(const char*, g_pszKey_AREA_CENTER, 0x008A8018);
CSF_GLOBAL(const char*, g_pszKey_AREA_RADIUS, 0x008A801C);
CSF_GLOBAL(const char*, g_pszKey_AREA_RADIUS_GAP, 0x008A8020);
CSF_GLOBAL(const char*, g_pszKey_AREA_HEIGHT, 0x008A8024);
CSF_GLOBAL(const char*, g_pszKey_AREA_EMIT_ANGLE, 0x008A8028);
CSF_GLOBAL(const char*, g_pszKey_ORIENT_TYPE, 0x008A802C);
CSF_GLOBAL(const char*, g_pszKey_SPARKS_MODE, 0x008A8030);
CSF_GLOBAL(const char*, g_pszKey_SPARKS_SIZE_FACTOR, 0x008A8034);
CSF_GLOBAL(const char*, g_pszKey_ORIENT_AT, 0x008A8038);
CSF_GLOBAL(const char*, g_pszKey_ORIENT_AT_BIAS, 0x008A803C);
CSF_GLOBAL(const char*, g_pszKey_ORIENT_UP, 0x008A8040);
CSF_GLOBAL(const char*, g_pszKey_ORIENT_UP_BIAS, 0x008A8044);

// The packet labels these two call destinations as vtables. They are not
// object vtable stores; retain the bindings without writing them into objects.
[[maybe_unused]] static const auto kFileInitializerPacketVtable = CSF_VTBL(0x00692B10);
[[maybe_unused]] static const auto kResourceReleasePacketVtable = CSF_VTBL(0x006DCF90);

static float& GfxTextureLoader_FloatField(std::uint32_t& field)
{
    return *reinterpret_cast<float*>(&field);
}

static std::uint8_t* GfxTextureLoader_FindEmitterProperty(
    std::uint8_t* definition, const char* keyword)
{
    return FindProperty(definition, nullptr, const_cast<char*>(keyword));
}

static int GfxTextureLoader_ReadEmitterInteger(std::uint8_t* property)
{
    return S_00692b80_ParseIntegerComponent(
        reinterpret_cast<S_00692b80*>(property), nullptr);
}

static float GfxTextureLoader_ReadEmitterFloat(std::uint8_t* property)
{
    return static_cast<float>(
        ReadComponentFloat(reinterpret_cast<std::uint32_t>(property)));
}

static float GfxTextureLoader_ReadEmitterVectorComponent(
    std::uint8_t* property, unsigned int index)
{
    auto* component = S_00692b80_GetComponent(
        reinterpret_cast<S_00692b80*>(property), nullptr, index);
    return static_cast<float>(
        ReadComponentFloat(reinterpret_cast<std::uint32_t>(component)));
}

#define EMITTER_FLOAT(field) GfxTextureLoader_FloatField(self->field)
#define EMITTER_PROPERTY(key) GfxTextureLoader_FindEmitterProperty(definition, key)
#define READ_FLOAT(key, field) \
    do { EMITTER_FLOAT(field) = GfxTextureLoader_ReadEmitterFloat(EMITTER_PROPERTY(key)); } while (0)
#define READ_OPTIONAL_FLOAT(key, field) \
    do { \
        auto* property = EMITTER_PROPERTY(key); \
        if (property) EMITTER_FLOAT(field) = GfxTextureLoader_ReadEmitterFloat(property); \
    } while (0)
#define READ_VECTOR2_BODY(property, x, y) \
    EMITTER_FLOAT(x) = GfxTextureLoader_ReadEmitterVectorComponent(property, 0); \
    EMITTER_FLOAT(y) = GfxTextureLoader_ReadEmitterVectorComponent(property, 1u)
#define READ_VECTOR3_BODY(property, x, y, z) \
    READ_VECTOR2_BODY(property, x, y); \
    EMITTER_FLOAT(z) = GfxTextureLoader_ReadEmitterVectorComponent(property, 2u)
#define READ_VECTOR2(key, x, y) \
    do { auto* property = EMITTER_PROPERTY(key); READ_VECTOR2_BODY(property, x, y); } while (0)
#define READ_VECTOR3(key, x, y, z) \
    do { auto* property = EMITTER_PROPERTY(key); READ_VECTOR3_BODY(property, x, y, z); } while (0)
#define READ_OPTIONAL_VECTOR2(key, x, y) \
    do { \
        auto* property = EMITTER_PROPERTY(key); \
        if (property) { READ_VECTOR2_BODY(property, x, y); } \
    } while (0)
#define READ_OPTIONAL_VECTOR3(key, x, y, z) \
    do { \
        auto* property = EMITTER_PROPERTY(key); \
        if (property) { READ_VECTOR3_BODY(property, x, y, z); } \
    } while (0)
#define READ_VECTOR4(key, x, y, z, w) \
    do { \
        auto* property = EMITTER_PROPERTY(key); \
        READ_VECTOR3_BODY(property, x, y, z); \
        EMITTER_FLOAT(w) = GfxTextureLoader_ReadEmitterVectorComponent(property, 3u); \
    } while (0)

static int __fastcall S_006dcf90_LoadEmitterDefinition(
    S_006dcf90* self, void* /*edx*/, char* resourceName)
{
    S_006dcf90_ResetDefaults(self, nullptr);

    alignas(8) std::uint8_t fileBytes[0x50];
    auto& file = *reinterpret_cast<S_00692b10*>(fileBytes);
    S_00692b10_InitializeFile(&file, nullptr);
    MenuExtras_LoadFile(&file, nullptr, resourceName);

    auto* definition = reinterpret_cast<std::uint8_t*>(file.field_0000);
    if (!definition)
    {
        MenuExtras_CloseFile(&file, nullptr);
        return 0;
    }

    auto* versionProperty = EMITTER_PROPERTY(g_pszKey_VERSION_gfx_texture_loader);
    const int version = versionProperty
        ? GfxTextureLoader_ReadEmitterInteger(versionProperty) : 1;
    if (version < 2)
    {
        char loaded = 0;
        if (version == 1)
            loaded = ParticleSystemDefs_ReadEmitterParameters(
                reinterpret_cast<float*>(self), nullptr, resourceName, definition);
        MenuExtras_CloseFile(&file, nullptr);
        return loaded;
    }

    self->field_0008 = GfxTextureLoader_ReadEmitterInteger(
        EMITTER_PROPERTY(g_pszKey_MAX_PARTICULAS));
    READ_VECTOR3(g_pszKey_AREA_EMISION, field_00f4, field_00f8, field_00fc);
    self->field_000c = GfxTextureLoader_ReadEmitterInteger(
        EMITTER_PROPERTY(g_pszKey_PART_PER_EMIS));
    if (auto* property = EMITTER_PROPERTY(g_pszKey_PART_PER_EMIS_BIAS))
        self->field_0088 = GfxTextureLoader_ReadEmitterInteger(property);

    READ_FLOAT(g_pszKey_EMISSION_DURATION, field_0014);
    READ_FLOAT(g_pszKey_GAP_EMIS, field_0010);
    if (EMITTER_FLOAT(field_0010) < g_MaximumHeadTimeStep)
        EMITTER_FLOAT(field_0010) = 0.029999999f;
    READ_OPTIONAL_FLOAT(g_pszKey_GAP_EMIS_BIAS, field_008c);
    READ_FLOAT(g_pszKey_PART_LIFE, field_0018);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_LIFE_BIAS, field_0090);

    const float duration = EMITTER_FLOAT(field_0014) == g_fZero
        ? EMITTER_FLOAT(field_0090) + EMITTER_FLOAT(field_0018)
        : EMITTER_FLOAT(field_0014) + EMITTER_FLOAT(field_0010);
    const auto particlesPerEmission =
        static_cast<std::int32_t>(self->field_000c + self->field_0088);
    const double estimatedParticles =
        duration / EMITTER_FLOAT(field_0010) * static_cast<double>(particlesPerEmission);
    if (static_cast<double>(static_cast<std::int32_t>(self->field_0008)) > estimatedParticles)
    {
        const auto rounded = static_cast<std::int64_t>(
            (estimatedParticles + g_MessageBoxAnimationScale) * g_flt_0082DC84);
        self->field_0008 = 10u * static_cast<std::uint32_t>(rounded);
    }

    READ_FLOAT(g_pszKey_PART_INIT_VELO, field_0024);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_INIT_VELO_BIAS, field_0094);
    READ_VECTOR3(g_pszKey_PART_INIT_DIR, field_0028, field_002c, field_0030);
    READ_OPTIONAL_VECTOR3(g_pszKey_PART_INIT_DIR_BIAS, field_0098, field_009c, field_00a0);

    if (g_fZero != EMITTER_FLOAT(field_0094) + EMITTER_FLOAT(field_0024)
        && static_cast<std::uint8_t>(Vector3ExactEquals(
            &EMITTER_FLOAT(field_0028), nullptr, &g_ZeroVectorX))
        && static_cast<std::uint8_t>(Vector3ExactEquals(
            &EMITTER_FLOAT(field_0098), nullptr, &g_ZeroVectorX)))
    {
        EMITTER_FLOAT(field_0028) = g_AxisY;
        EMITTER_FLOAT(field_002c) = g_AxisY_Y;
        EMITTER_FLOAT(field_0030) = g_AxisY_Z;
    }

    READ_VECTOR3(g_pszKey_FORCE_EMIS, field_0064, field_0068, field_006c);
    READ_OPTIONAL_FLOAT(g_pszKey_FRICCION, field_0084);
    READ_FLOAT(g_pszKey_PART_2D_ROTATION_START, field_001c);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_2D_ROTATION_START_BIAS, field_00cc);
    READ_FLOAT(g_pszKey_PART_2D_ROTATION_END, field_0020);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_2D_ROTATION_END_BIAS, field_00d0);
    READ_VECTOR4(g_pszKey_PART_START_COLOR, field_0044, field_0048, field_004c, field_0050);
    READ_VECTOR4(g_pszKey_PART_END_COLOR, field_0054, field_0058, field_005c, field_0060);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_START_COLOR_ALPHA_BIAS, field_00c0);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_END_COLOR_ALPHA_BIAS, field_00c4);
    READ_OPTIONAL_FLOAT(g_pszKey_PART_COLOR_BIAS, field_00c8);
    READ_VECTOR2(g_pszKey_PART_START_SIZE, field_0034, field_0038);
    READ_OPTIONAL_VECTOR2(g_pszKey_PART_START_SIZE_BIAS, field_00a4, field_00a8);
    READ_VECTOR2(g_pszKey_PART_END_SIZE, field_003c, field_0040);
    READ_OPTIONAL_VECTOR2(g_pszKey_PART_END_SIZE_BIAS, field_00ac, field_00b0);
    READ_OPTIONAL_VECTOR2(g_pszKey_PART_SIZE_SCALE_BIAS, field_00b4, field_00b8);
    if (auto* property = EMITTER_PROPERTY(g_pszKey_USE_REGULAR_SIZE_SCALE))
        self->field_00bc = GfxTextureLoader_ReadEmitterInteger(property) != 0;
    READ_OPTIONAL_VECTOR2(g_pszKey_PART_UV0, field_0074, field_0078);
    READ_OPTIONAL_VECTOR2(g_pszKey_PART_UV1, field_007c, field_0080);

    char textureName[256];
    char modelName[256];
    char resolvedName[256];

    auto* textureProperty = EMITTER_PROPERTY(g_pszKey_TEXTURE_gfx_texture_loader);
    CopyAttributeText(textureProperty, nullptr, textureName, 256);
    if (std::strlen(textureName) != 0)
    {
        ResourceName_Transform(
            resourceName, const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
            resolvedName, 256);
        GfxTextureLoader_AppendResourceName(
            resolvedName, 256, textureName, 0, 0, 0, 0, 0, 0, 0);
        GfxTextureLoader_AppendResourceName(
            resolvedName, 256, reinterpret_cast<char*>(&g_ParticleReadParam),
            0, 0, 0, 0, 0, 0, 0);

        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* fileProvider =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, accessor, 0)[16 / 4])(accessor, nullptr);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, fileProvider, 0)[28 / 4])(
                    fileProvider, nullptr, resolvedName))
        {
            self->field_0000 = GlobalInt_FindNameId(
                reinterpret_cast<std::uint8_t*>(textureName));
        }
    }

    std::memset(modelName, 0, 256);
    self->field_0004 = 0;
    if (auto* property = EMITTER_PROPERTY(g_pszKey_MODEL))
        CopyAttributeText(property, nullptr, modelName, 256);
    if (std::strlen(modelName) != 0)
    {
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        auto* provider = reinterpret_cast<S_006d4450*>(
            GetCachedObjectProvider(accessor, nullptr));
        const int modelIndex =
            GfxTextureLoader_FindModelIndex(provider, nullptr, modelName, -1);
        if (modelIndex != -1)
        {
            S_006dcf90_ReleaseResource(self, nullptr);
            const int resource =
                GfxObject_AcquireIndexedResource(provider, nullptr, modelIndex);
            self->field_0004 = resource;
            GetGlobalValue_6C5760();
            GfxSkin_ForwardWithSkinVisitorCallback(resource);
        }
    }

    if (auto* property = EMITTER_PROPERTY(g_pszKey_ADITIVE))
        self->field_0070 = GfxTextureLoader_ReadEmitterInteger(property) != 0;

    if (auto* property = EMITTER_PROPERTY(g_pszKey_BBOX))
    {
        const float x0 = GfxTextureLoader_ReadEmitterVectorComponent(property, 0);
        const float y0 = GfxTextureLoader_ReadEmitterVectorComponent(property, 1u);
        const float z0 = GfxTextureLoader_ReadEmitterVectorComponent(property, 2u);
        const float x1 = GfxTextureLoader_ReadEmitterVectorComponent(property, 3u);
        const float y1 = GfxTextureLoader_ReadEmitterVectorComponent(property, 4u);
        const float z1 = GfxTextureLoader_ReadEmitterVectorComponent(property, 5u);
        EMITTER_FLOAT(field_013c) = x0 >= x1 ? x1 : x0;
        EMITTER_FLOAT(field_0140) = y0 < y1 ? y0 : y1;
        EMITTER_FLOAT(field_0144) = z0 < z1 ? z0 : z1;
        EMITTER_FLOAT(field_0148) = x0 < x1 ? x1 : x0;
        EMITTER_FLOAT(field_014c) = y0 < y1 ? y1 : y0;
        EMITTER_FLOAT(field_0150) = z0 < z1 ? z1 : z0;
    }

    if (auto* property = EMITTER_PROPERTY(g_pszKey_AREA_TYPE))
    {
        CopyAttributeText(property, nullptr, resolvedName, 256);
        self->field_00d4 = GfxTextureLoader_FindFilterType(resolvedName);
        if (self->field_00d4 == static_cast<std::uint32_t>(-1))
            self->field_00d4 = 0;
    }
    READ_OPTIONAL_VECTOR3(g_pszKey_AREA_CENTER, field_00d8, field_00dc, field_00e0);
    if (auto* property = EMITTER_PROPERTY(g_pszKey_AREA_RADIUS))
        EMITTER_FLOAT(field_00e4) = static_cast<float>(std::fabs(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(property))));
    READ_OPTIONAL_FLOAT(g_pszKey_AREA_RADIUS_GAP, field_00e8);
    READ_OPTIONAL_FLOAT(g_pszKey_AREA_HEIGHT, field_00ec);
    READ_OPTIONAL_FLOAT(g_pszKey_AREA_EMIT_ANGLE, field_00f0);

    if (auto* property = EMITTER_PROPERTY(g_pszKey_ORIENT_TYPE))
    {
        CopyAttributeText(property, nullptr, resolvedName, 256);
        self->field_0100 = GfxTextureLoader_FindFacingType(resolvedName);
        if (self->field_0100 == static_cast<std::uint32_t>(-1))
            self->field_0100 = 0;
    }
    if (auto* property = EMITTER_PROPERTY(g_pszKey_SPARKS_MODE))
    {
        const std::uint8_t sparksMode =
            GfxTextureLoader_ReadEmitterInteger(property) != 0;
        self->field_0104 = sparksMode;
        if (sparksMode == 1 && !self->field_0100)
            self->field_0104 = 0;
    }
    READ_OPTIONAL_FLOAT(g_pszKey_SPARKS_SIZE_FACTOR, field_0108);
    READ_OPTIONAL_VECTOR3(g_pszKey_ORIENT_AT, field_010c, field_0110, field_0114);
    READ_OPTIONAL_VECTOR3(g_pszKey_ORIENT_UP, field_0118, field_011c, field_0120);
    READ_OPTIONAL_VECTOR3(g_pszKey_ORIENT_AT_BIAS, field_0124, field_0128, field_012c);
    READ_OPTIONAL_VECTOR3(g_pszKey_ORIENT_UP_BIAS, field_0130, field_0134, field_0138);

    MenuExtras_CloseFile(&file, nullptr);
    return 1;
}

#undef EMITTER_FLOAT
#undef EMITTER_PROPERTY
#undef READ_FLOAT
#undef READ_OPTIONAL_FLOAT
#undef READ_VECTOR2_BODY
#undef READ_VECTOR3_BODY
#undef READ_VECTOR2
#undef READ_VECTOR3
#undef READ_OPTIONAL_VECTOR2
#undef READ_OPTIONAL_VECTOR3
#undef READ_VECTOR4

HOOK(0x006DD340, S_006dcf90_LoadEmitterDefinition, "auto");
