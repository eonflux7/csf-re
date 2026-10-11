#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpUserDataArrayGetName, 0x005870B0, (int a1));
CSF_FUNC(int, __cdecl, RpMaterialGetUserDataArrayCount, 0x0058BD60, (int a1));
CSF_FUNC(int, __cdecl, RpMaterialGetUserDataArray, 0x0058BD90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpUserDataArrayGetInt, 0x0058BE50, (int a1, int a2));

CSF_GLOBAL(std::uint32_t, g_LightPropertiesOffset, 0x008BB484);

static bool GfxObjectProps_MatchesPropertyName(
    const char* name, const char* expected, std::size_t byteCount)
{
    for (std::size_t i = 0; i < byteCount; ++i)
    {
        if (name[i] != expected[i])
            return false;
    }
    return true;
}

static int __cdecl GfxObjectProps_UpdateLightType(int objectAddress)
{
    void* object = reinterpret_cast<void*>(objectAddress);
    const int material = CSF_AT(int, object, 0x4);
    if (!material)
        return objectAddress;

    int* properties = CSF_AT(int*, object, g_LightPropertiesOffset + 4);
    int result = RpMaterialGetUserDataArrayCount(material);
    if (result == 0)
        return result;

    result = static_cast<int>(static_cast<std::uint32_t>(result) - 1u);
    bool lightmapped = false;
    int lightType = -1;

    if (result >= 0)
    {
        do
        {
            const int userData = RpMaterialGetUserDataArray(material, result);
            const char* name =
                reinterpret_cast<const char*>(RpUserDataArrayGetName(userData));

            if (name)
            {
                if (GfxObjectProps_MatchesPropertyName(
                        name, CSF_STR(0x008603C4, "tipoLuz"), 8))
                {
                    lightType = RpUserDataArrayGetInt(userData, 0);
                }
                else if (GfxObjectProps_MatchesPropertyName(
                             name, CSF_STR(0x008603B4, "flagLightmap"), 13))
                {
                    lightmapped = RpUserDataArrayGetInt(userData, 0) != 0;
                }
                else if (GfxObjectProps_MatchesPropertyName(
                             name, CSF_STR(0x008603A4, "flagDinamica"), 13))
                {
                    RpUserDataArrayGetInt(userData, 0);
                }
            }

            --result;
        } while (result >= 0);

        result = lightType;
        if (lightType >= 0)
        {
            result = lightType - 1;
            CSF_AT(int, properties, 0) = result;
            return result;
        }
    }

    CSF_AT(int, properties, 0) = lightmapped ? 2 : 0;
    return result;
}

HOOK(0x006B86A0, GfxObjectProps_UpdateLightType, "auto");
