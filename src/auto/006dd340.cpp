#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vector3ExactEquals, 0x004069B0, (float* self, void* , float* other));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
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
CSF_FUNC(int, __fastcall, GfxObject_AcquireIndexedResource, 0x006D4740, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxTextureLoader_FindModelIndex, 0x006D58E0, (void* self, void* edx, char* String2, int a3));
CSF_FUNC(int, __cdecl, GfxTextureLoader_FindFilterType, 0x006DCEF0, (char* name));
CSF_FUNC(int, __cdecl, GfxTextureLoader_FindFacingType, 0x006DCF40, (char* name));
CSF_FUNC(int, __fastcall, S_006dcf90_ReleaseResource, 0x006DCF90, (S_006dcf90* self, void*));
CSF_FUNC(int, __fastcall, S_006dcf90_ResetParameters, 0x006DD040, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ParticleSystemDefs_ReadEmitterParameters, 0x00701EF0, (float* self, void* , char* resourceName, std::uint8_t* definition));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_MaximumHeadTimeStep, 0x0082DC68);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(std::uint32_t, g_ParticleReadParam, 0x0086103C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
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
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

static float& GfxTextureLoader_FloatField(std::uint32_t& field)
{
    return reinterpret_cast<float&>(field);
}

static std::uint32_t GfxTextureLoader_ComponentAddress(std::uint8_t* node)
{
    return reinterpret_cast<std::uint32_t>(node);
}

static std::uint8_t* GfxTextureLoader_FindProperty(std::uint8_t* root, const char* key)
{
    return FindProperty(root, nullptr, const_cast<char*>(key));
}

static void GfxTextureLoader_ReadFloatProperty(
    std::uint8_t* root, const char* key, std::uint32_t& field, bool optional = false)
{
    auto* property = GfxTextureLoader_FindProperty(root, key);
    if (!optional || property)
        GfxTextureLoader_FloatField(field) =
            static_cast<float>(ReadComponentFloat(GfxTextureLoader_ComponentAddress(property)));
}

static void GfxTextureLoader_ReadVector2Property(
    std::uint8_t* root, const char* key,
    std::uint32_t& x, std::uint32_t& y, bool optional = false)
{
    auto* property = GfxTextureLoader_FindProperty(root, key);
    if (optional && !property)
        return;
    GfxTextureLoader_FloatField(x) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 0)));
    GfxTextureLoader_FloatField(y) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 1u)));
}

static void GfxTextureLoader_ReadVector3Property(
    std::uint8_t* root, const char* key,
    std::uint32_t& x, std::uint32_t& y, std::uint32_t& z, bool optional = false)
{
    auto* property = GfxTextureLoader_FindProperty(root, key);
    if (optional && !property)
        return;
    GfxTextureLoader_FloatField(x) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 0)));
    GfxTextureLoader_FloatField(y) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 1u)));
    GfxTextureLoader_FloatField(z) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 2u)));
}

static void GfxTextureLoader_ReadColorProperty(
    std::uint8_t* root, const char* key,
    std::uint32_t& r, std::uint32_t& g, std::uint32_t& b, std::uint32_t& a)
{
    auto* property = GfxTextureLoader_FindProperty(root, key);
    GfxTextureLoader_FloatField(r) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 0)));
    GfxTextureLoader_FloatField(g) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 1u)));
    GfxTextureLoader_FloatField(b) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 2u)));
    GfxTextureLoader_FloatField(a) =
        static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 3u)));
}

static int __fastcall S_006dcf90_LoadEmitterDefinition(
    S_006dcf90* self, void* /*edx*/, char* resourceName)
{
    S_006dcf90_ResetParameters(self, nullptr);

    alignas(8) std::uint8_t fileBytes[0x50];
    // These entries were classified as vtables in the packet;
    // they are call targets, not vtable stores.
    (void)CSF_VTBL(0x00692B10);
    (void)CSF_VTBL(0x006DCF90);

    MenuExtras_InitializeFile(fileBytes, nullptr);
    MenuExtras_LoadFile(fileBytes, nullptr, resourceName);
    auto* root = CSF_AT(std::uint8_t*, fileBytes, 0x00);
    if (!root) {
        MenuExtras_CloseFile(fileBytes, nullptr);
        return 0;
    }

    auto* property = GfxTextureLoader_FindProperty(root, g_pszKey_VERSION_gfx_texture_loader);
    int version = property
        ? ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property)) : 1;
    if (version < 2) {
        char loaded = 0;
        if (version == 1)
            loaded = ParticleSystemDefs_ReadEmitterParameters(
                reinterpret_cast<float*>(self), nullptr, resourceName, root);
        MenuExtras_CloseFile(fileBytes, nullptr);
        return loaded;
    }

    property = GfxTextureLoader_FindProperty(root, g_pszKey_MAX_PARTICULAS);
    self->field_0008 = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property));
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_AREA_EMISION,
        self->field_00f4, self->field_00f8, self->field_00fc);

    property = GfxTextureLoader_FindProperty(root, g_pszKey_PART_PER_EMIS);
    self->field_000c = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property));
    property = GfxTextureLoader_FindProperty(root, g_pszKey_PART_PER_EMIS_BIAS);
    if (property)
        self->field_0088 = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property));

    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_EMISSION_DURATION, self->field_0014);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_GAP_EMIS, self->field_0010);
    if (GfxTextureLoader_FloatField(self->field_0010) < g_MaximumHeadTimeStep)
        GfxTextureLoader_FloatField(self->field_0010) = 0.029999999f;
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_GAP_EMIS_BIAS, self->field_008c, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_LIFE, self->field_0018);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_LIFE_BIAS, self->field_0090, true);

    float activeDuration = GfxTextureLoader_FloatField(self->field_0014) == g_fZero
        ? GfxTextureLoader_FloatField(self->field_0090) + GfxTextureLoader_FloatField(self->field_0018)
        : GfxTextureLoader_FloatField(self->field_0014) + GfxTextureLoader_FloatField(self->field_0010);
    double estimatedCount = activeDuration / GfxTextureLoader_FloatField(self->field_0010)
        * static_cast<double>(static_cast<std::int32_t>(self->field_000c + self->field_0088));
    if (static_cast<double>(static_cast<std::int32_t>(self->field_0008)) > estimatedCount) {
        auto roundedCount = static_cast<std::int64_t>(
            (estimatedCount + g_MessageBoxAnimationScale) * g_flt_0082DC84);
        self->field_0008 = 10u * static_cast<std::uint32_t>(roundedCount);
    }

    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_INIT_VELO, self->field_0024);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_INIT_VELO_BIAS, self->field_0094, true);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_PART_INIT_DIR,
        self->field_0028, self->field_002c, self->field_0030);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_PART_INIT_DIR_BIAS,
        self->field_0098, self->field_009c, self->field_00a0, true);

    if (g_fZero != GfxTextureLoader_FloatField(self->field_0094)
            + GfxTextureLoader_FloatField(self->field_0024)
        && static_cast<std::uint8_t>(Vector3ExactEquals(
            &GfxTextureLoader_FloatField(self->field_0028), nullptr, &g_ZeroVectorX))
        && static_cast<std::uint8_t>(Vector3ExactEquals(
            &GfxTextureLoader_FloatField(self->field_0098), nullptr, &g_ZeroVectorX))) {
        GfxTextureLoader_FloatField(self->field_0028) = g_AxisY;
        GfxTextureLoader_FloatField(self->field_002c) = g_AxisY_Y;
        GfxTextureLoader_FloatField(self->field_0030) = g_AxisY_Z;
    }

    GfxTextureLoader_ReadVector3Property(root, g_pszKey_FORCE_EMIS,
        self->field_0064, self->field_0068, self->field_006c);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_FRICCION, self->field_0084, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_2D_ROTATION_START, self->field_001c);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_2D_ROTATION_START_BIAS, self->field_00cc, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_2D_ROTATION_END, self->field_0020);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_2D_ROTATION_END_BIAS, self->field_00d0, true);

    GfxTextureLoader_ReadColorProperty(root, g_pszKey_PART_START_COLOR,
        self->field_0044, self->field_0048, self->field_004c, self->field_0050);
    GfxTextureLoader_ReadColorProperty(root, g_pszKey_PART_END_COLOR,
        self->field_0054, self->field_0058, self->field_005c, self->field_0060);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_START_COLOR_ALPHA_BIAS, self->field_00c0, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_END_COLOR_ALPHA_BIAS, self->field_00c4, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_PART_COLOR_BIAS, self->field_00c8, true);

    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_START_SIZE,
        self->field_0034, self->field_0038);
    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_START_SIZE_BIAS,
        self->field_00a4, self->field_00a8, true);
    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_END_SIZE,
        self->field_003c, self->field_0040);
    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_END_SIZE_BIAS,
        self->field_00ac, self->field_00b0, true);
    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_SIZE_SCALE_BIAS,
        self->field_00b4, self->field_00b8, true);
    property = GfxTextureLoader_FindProperty(root, g_pszKey_USE_REGULAR_SIZE_SCALE);
    if (property)
        self->field_00bc = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property)) != 0;

    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_UV0,
        self->field_0074, self->field_0078, true);
    GfxTextureLoader_ReadVector2Property(root, g_pszKey_PART_UV1,
        self->field_007c, self->field_0080, true);

    char textureName[256];
    char modelName[256];
    char resolvedName[256];
    property = GfxTextureLoader_FindProperty(root, g_pszKey_TEXTURE_gfx_texture_loader);
    CopyAttributeText(property, nullptr, textureName, 256);
    if (std::strlen(textureName) != 0) {
        ResourceName_Transform(resourceName,
            const_cast<char*>(CSF_STR(0x00860C74, "Textures/")), resolvedName, 256);
        GfxTextureLoader_AppendResourceName(
            resolvedName, 256, textureName, 0, 0, 0, 0, 0, 0, 0);
        GfxTextureLoader_AppendResourceName(
            resolvedName, 256, reinterpret_cast<char*>(&g_ParticleReadParam),
            0, 0, 0, 0, 0, 0, 0);

        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
        void* resources = reinterpret_cast<void*>(
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[16 / 4])(
                accessor, nullptr));
        if (((bool(__fastcall*)(void*, void*, char*))CSF_AT(void**, resources, 0)[28 / 4])(
                resources, nullptr, resolvedName)) {
            self->field_0000 = GlobalInt_FindNameId(
                reinterpret_cast<std::uint8_t*>(textureName));
        }
    }

    std::memset(modelName, 0, sizeof(modelName));
    self->field_0004 = 0;
    property = GfxTextureLoader_FindProperty(root, g_pszKey_MODEL);
    if (property)
        CopyAttributeText(property, nullptr, modelName, 256);
    if (std::strlen(modelName) != 0) {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
        void* provider = GetCachedObjectProvider(accessor, nullptr);
        int modelIndex = GfxTextureLoader_FindModelIndex(provider, nullptr, modelName, -1);
        if (modelIndex != -1) {
            S_006dcf90_ReleaseResource(self, nullptr);
            int resource = GfxObject_AcquireIndexedResource(provider, nullptr, modelIndex);
            self->field_0004 = resource;
            GetGlobalValue_6C5760();
            GfxSkin_ForwardWithSkinVisitorCallback(resource);
        }
    }

    property = GfxTextureLoader_FindProperty(root, g_pszKey_ADITIVE);
    if (property)
        self->field_0070 = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property)) != 0;

    property = GfxTextureLoader_FindProperty(root, g_pszKey_BBOX);
    if (property) {
        float x1 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 0)));
        float y1 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 1u)));
        float z1 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 2u)));
        float x2 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 3u)));
        float y2 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 4u)));
        float z2 = static_cast<float>(ReadComponentFloat(GetComponent(property, nullptr, 5u)));
        GfxTextureLoader_FloatField(self->field_013c) = x1 >= x2 ? x2 : x1;
        GfxTextureLoader_FloatField(self->field_0140) = y1 < y2 ? y1 : y2;
        GfxTextureLoader_FloatField(self->field_0144) = z1 < z2 ? z1 : z2;
        GfxTextureLoader_FloatField(self->field_0148) = x1 < x2 ? x2 : x1;
        GfxTextureLoader_FloatField(self->field_014c) = y1 < y2 ? y2 : y1;
        GfxTextureLoader_FloatField(self->field_0150) = z1 < z2 ? z2 : z1;
    }

    property = GfxTextureLoader_FindProperty(root, g_pszKey_AREA_TYPE);
    if (property) {
        CopyAttributeText(property, nullptr, resolvedName, 256);
        self->field_00d4 = GfxTextureLoader_FindFilterType(resolvedName);
        if (self->field_00d4 == static_cast<std::uint32_t>(-1))
            self->field_00d4 = 0;
    }
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_AREA_CENTER,
        self->field_00d8, self->field_00dc, self->field_00e0, true);
    property = GfxTextureLoader_FindProperty(root, g_pszKey_AREA_RADIUS);
    if (property)
        GfxTextureLoader_FloatField(self->field_00e4) = static_cast<float>(
            std::fabs(ReadComponentFloat(GfxTextureLoader_ComponentAddress(property))));
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_AREA_RADIUS_GAP, self->field_00e8, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_AREA_HEIGHT, self->field_00ec, true);
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_AREA_EMIT_ANGLE, self->field_00f0, true);

    property = GfxTextureLoader_FindProperty(root, g_pszKey_ORIENT_TYPE);
    if (property) {
        CopyAttributeText(property, nullptr, resolvedName, 256);
        self->field_0100 = GfxTextureLoader_FindFacingType(resolvedName);
        if (self->field_0100 == static_cast<std::uint32_t>(-1))
            self->field_0100 = 0;
    }
    property = GfxTextureLoader_FindProperty(root, g_pszKey_SPARKS_MODE);
    if (property) {
        self->field_0104 = ParseIntegerComponent(GfxTextureLoader_ComponentAddress(property)) != 0;
        if (self->field_0104 == 1 && !self->field_0100)
            self->field_0104 = 0;
    }
    GfxTextureLoader_ReadFloatProperty(root, g_pszKey_SPARKS_SIZE_FACTOR, self->field_0108, true);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_ORIENT_AT,
        self->field_010c, self->field_0110, self->field_0114, true);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_ORIENT_UP,
        self->field_0118, self->field_011c, self->field_0120, true);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_ORIENT_AT_BIAS,
        self->field_0124, self->field_0128, self->field_012c, true);
    GfxTextureLoader_ReadVector3Property(root, g_pszKey_ORIENT_UP_BIAS,
        self->field_0130, self->field_0134, self->field_0138, true);

    MenuExtras_CloseFile(fileBytes, nullptr);
    return 1;
}

HOOK(0x006DD340, S_006dcf90_LoadEmitterDefinition, "auto");
