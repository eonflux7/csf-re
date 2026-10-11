#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxObjectProps_SetFlag, 0x0040ED00, (void* self, void* edx, int a2, char a3));
CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetName, 0x005870B0, (int a1));
CSF_FUNC(double, __cdecl, RpUserDataArrayGetReal, 0x0058BE40, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetInt, 0x0058BE50, (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_EnsureExtendedProperties, 0x006B8020, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GfxObjectProps_InternString, 0x006BA370, (void* self, void* edx, void* a2));
CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));
CSF_FUNC(int, __cdecl, _isdigit, 0x007B1966, (int C));
CSF_FUNC(int, __cdecl, _isspace, 0x007B19DF, (int C));
CSF_FUNC(char*, __cdecl, _strstr, 0x007B2350, (char* Str, char* SubStr));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_ObjectPropertiesPluginOffset, 0x008BB474);

// The packet also classifies this direct-call address as a vtable reference.
static const auto g_ObjectPropertiesVtableBinding = CSF_VTBL(0x006B8020);

// The original compares property names inline rather than calling strcmp.
static bool GfxObjectProps_NameEquals(const char* name, const char* expected)
{
    for (;;)
    {
        if (*name != *expected)
            return false;
        if (*expected == 0)
            return true;
        ++name;
        ++expected;
    }
}

static char __cdecl GfxObjectProps_ParseUserData(
    S_006b8020* properties,
    float* bounds,
    int* orderNumber,
    bool* doubleSided,
    int userData,
    int /*unused6*/,
    int /*unused7*/)
{
    const char* name =
        reinterpret_cast<const char*>(RpUserDataArrayGetName(userData));

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604F8, "datosRW")))
    {
        char* text = reinterpret_cast<char*>(RpUserDataArrayGetInt(userData, 0));
        int tagId = -1;
        char* tag = _strstr(text, const_cast<char*>(CSF_STR(0x008604F0, "tag=")));
        if (tag)
        {
            char* digits = tag + 4;
            if (_isspace(static_cast<signed char>(*digits)))
            {
                do
                {
                    ++digits;
                } while (_isspace(static_cast<signed char>(*digits)));
            }

            char* end = digits;
            if (_isdigit(static_cast<signed char>(*end)))
            {
                do
                {
                    ++end;
                } while (_isdigit(static_cast<signed char>(*end)));
            }
            *end = 0;
            tagId = ParseThumbnailIndex(digits);
        }
        CSF_AT(int, properties, 0x00) = tagId;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604E8, "tagID")))
    {
        CSF_AT(int, properties, 0x00) = RpUserDataArrayGetInt(userData, 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604DC, "ordNumber")))
    {
        *orderNumber = RpUserDataArrayGetInt(userData, 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604D0, "bMipmaps")))
    {
        int value = RpUserDataArrayGetInt(userData, 0);
        GfxObjectProps_SetFlag(
            reinterpret_cast<uint8_t*>(properties) + 4, nullptr, 64, value != 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604C8, "bEsAgua")))
    {
        int value = RpUserDataArrayGetInt(userData, 0);
        GfxObjectProps_SetFlag(
            reinterpret_cast<uint8_t*>(properties) + 4, nullptr, 2, value != 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604B4, "bEsCristalRompible")))
    {
        int value = RpUserDataArrayGetInt(userData, 0);
        GfxObjectProps_SetFlag(
            reinterpret_cast<uint8_t*>(properties) + 4, nullptr, 128, value != 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008604A4, "bEsBackPlane")))
    {
        int value = RpUserDataArrayGetInt(userData, 0);
        GfxObjectProps_SetFlag(
            reinterpret_cast<uint8_t*>(properties) + 4, nullptr, 256, value != 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860494, "bTieneAnimacion")))
    {
        int value = RpUserDataArrayGetInt(userData, 0);
        GfxObjectProps_SetFlag(
            reinterpret_cast<uint8_t*>(properties) + 4, nullptr, 512, value != 0);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860488, "tipoAlpha")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t value = static_cast<uint32_t>(RpUserDataArrayGetInt(userData, 0));
        CSF_AT(uint8_t, reinterpret_cast<void*>(properties->field_0010), 0x0C) =
            static_cast<uint8_t>(value - 1);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x0086047C, "bDobleCara")))
    {
        *doubleSided = RpUserDataArrayGetInt(userData, 0) != 0;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860470, "planoAlpha")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t value = static_cast<uint32_t>(RpUserDataArrayGetInt(userData, 0));
        CSF_AT(uint8_t, reinterpret_cast<void*>(properties->field_0010), 0x0D) =
            static_cast<uint8_t>(value - 1);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860464, "nivelTest")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        int value = RpUserDataArrayGetInt(userData, 0);
        CSF_AT(int, reinterpret_cast<void*>(properties->field_0010), 0x10) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860458, "strIdent")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t* text =
            reinterpret_cast<uint32_t*>(RpUserDataArrayGetInt(userData, 0));
        alignas(8) uint8_t stringBytes[0x20];
        C_0082d410& temporaryString =
            *reinterpret_cast<C_0082d410*>(stringBytes);
        C_0082d410_ConstructFromText(&temporaryString, nullptr, text);
        void* interned = GfxObjectProps_InternString(
            CSF_AT(S_006b9580*, g_pRwEngineInstance, g_ObjectPropertiesPluginOffset),
            nullptr, &temporaryString);
        CSF_AT(uint32_t, reinterpret_cast<void*>(properties->field_0010), 0x00) =
            reinterpret_cast<uint32_t>(interned);
        CMenuSaveGame_ResetStringStorage(&temporaryString, nullptr);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x0086044C, "strMaterial")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t* text =
            reinterpret_cast<uint32_t*>(RpUserDataArrayGetInt(userData, 0));
        alignas(8) uint8_t stringBytes[0x20];
        C_0082d410& temporaryString =
            *reinterpret_cast<C_0082d410*>(stringBytes);
        C_0082d410_ConstructFromText(&temporaryString, nullptr, text);
        void* interned = GfxObjectProps_InternString(
            CSF_AT(S_006b9580*, g_pRwEngineInstance, g_ObjectPropertiesPluginOffset),
            nullptr, &temporaryString);
        CSF_AT(uint32_t, reinterpret_cast<void*>(properties->field_0010), 0x04) =
            reinterpret_cast<uint32_t>(interned);
        CMenuSaveGame_ResetStringStorage(&temporaryString, nullptr);
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860440, "fDistancia")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        float value = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        CSF_AT(float, reinterpret_cast<void*>(properties->field_0010), 0x1C) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860434, "fDistMin")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        float value = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        CSF_AT(float, reinterpret_cast<void*>(properties->field_0010), 0x20) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860428, "fDistAlpha")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        float value = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        CSF_AT(float, reinterpret_cast<void*>(properties->field_0010), 0x24) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860420, "fAnimU")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        float value = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        CSF_AT(float, reinterpret_cast<void*>(properties->field_0010), 0x28) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x00860418, "fAnimV")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        float value = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        CSF_AT(float, reinterpret_cast<void*>(properties->field_0010), 0x2C) = value;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x0086040C, "tipoObjeto")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t value = static_cast<uint32_t>(RpUserDataArrayGetInt(userData, 0));
        CSF_AT(uint32_t, reinterpret_cast<void*>(properties->field_0010), 0x14) =
            value - 1;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603FC, "subtipoObjeto")))
    {
        GfxObjectProps_EnsureExtendedProperties(properties, nullptr);
        uint32_t value = static_cast<uint32_t>(RpUserDataArrayGetInt(userData, 0));
        CSF_AT(uint32_t, reinterpret_cast<void*>(properties->field_0010), 0x18) =
            value - 1;
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603F4, "xmin")))
    {
        bounds[0] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603EC, "ymin")))
    {
        bounds[1] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603E4, "zmin")))
    {
        bounds[2] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603DC, "xmax")))
    {
        bounds[3] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603D4, "ymax")))
    {
        bounds[4] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    if (GfxObjectProps_NameEquals(name, CSF_STR(0x008603CC, "zmax")))
    {
        bounds[5] = static_cast<float>(RpUserDataArrayGetReal(userData, 0));
        return 1;
    }

    return 0;
}

HOOK(0x006BA6D0, GfxObjectProps_ParseUserData, "auto");
