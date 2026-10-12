#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_GetTintColorDwords, 0x006B40D0, (C_00860288* self, void* , uint32_t* outColor));
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __cdecl, GfxProps_ReadPropertyTableEntry, 0x006B88C0, (int a1));
CSF_FUNC(unsigned int, __stdcall, GfxLights_ApplyFilteredLightProperties, 0x006D1050, (unsigned int* a1));
CSF_FUNC(float*, __fastcall, GfxLights_CombineTintColors, 0x006D1150, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_00861988_SetTertiaryParameters, 0x006FD080, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);

static char __fastcall GfxLights_FilterLightPropertiesAndApplyTint(
    void* self, void* /*edx*/, unsigned int* properties, int skin)
{
    if (properties[0] == 0 && properties[1] == 0)
        return 0;

    const uint8_t disabled = CSF_AT(uint8_t, self, 0x19);
    if (disabled != 0)
        return static_cast<char>(disabled);

    alignas(8) uint8_t combinedColorBytes[0x10];
    alignas(8) uint8_t tintColorBytes[0x10];
    alignas(8) uint8_t colorBytes[0x18];
    auto* color = reinterpret_cast<uint32_t*>(colorBytes);

    const void* sourceColor = CSF_AT(uint8_t, self, 0x1A)
        ? static_cast<void*>(static_cast<uint8_t*>(self) + 0x1C)
        : static_cast<void*>(&g_ColorBlack);

    color[0] = CSF_AT(uint32_t, sourceColor, 0);
    color[1] = CSF_AT(uint32_t, sourceColor, 4);
    color[2] = CSF_AT(uint32_t, sourceColor, 8);
    color[3] = CSF_AT(uint32_t, sourceColor, 0xC);

    if (skin != 0)
    {
        const int owner = GfxSkin_GetDistanceOverrideOwner(skin);
        if (owner != 0)
        {
            auto* tintOwner = CSF_AT(
                C_00860288*, reinterpret_cast<void*>(owner), 0x10);
            if (tintOwner->vIsField0088Set())
            {
                uint32_t* tint = C_00860288_GetTintColorDwords(
                    tintOwner, nullptr, reinterpret_cast<uint32_t*>(tintColorBytes));
                float* combined = GfxLights_CombineTintColors(
                    colorBytes, nullptr,
                    reinterpret_cast<float*>(combinedColorBytes),
                    reinterpret_cast<float*>(tint));
                color[0] = CSF_AT(uint32_t, combined, 0);
                color[1] = CSF_AT(uint32_t, combined, 4);
                color[2] = CSF_AT(uint32_t, combined, 8);
                color[3] = CSF_AT(uint32_t, combined, 0xC);
            }
        }
    }

    unsigned int acceptedCount = 0;
    if (properties[0] != 0)
    {
        void* cachedProvider = GetCachedObjectProvider(
            CSF_AT(void*, self, 0x30), nullptr);
        unsigned int index = 0;
        unsigned int* entry = properties + 2;

        if (properties[0] != 0)
        {
            do
            {
                if (acceptedCount >= CSF_AT(uint32_t, self, 0x14))
                    break;

                void* object = reinterpret_cast<void*>(*entry);
                void* tableEntry = reinterpret_cast<void*>(
                    GfxProps_ReadPropertyTableEntry(static_cast<int>(*entry)));

                if (CSF_AT(uint8_t, object, 1) == 1)
                {
                    const int type =
                        reinterpret_cast<int (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, tableEntry, 0)[0x1C / 4])(
                                tableEntry, nullptr);
                    if (type == 3)
                        ++acceptedCount;
                    else
                        *entry = 0;
                }
                else if (object != cachedProvider)
                {
                    C_00861988_SetTertiaryParameters(tableEntry, nullptr, color);
                }
                else
                {
                    *entry = 0;
                }

                ++index;
                ++entry;
            } while (index < properties[0]);
        }

        if (index < properties[0])
        {
            entry = properties + index + 2;
            do
            {
                void* object = reinterpret_cast<void*>(*entry);
                if (CSF_AT(uint8_t, object, 1) == 1 || object == cachedProvider)
                    *entry = 0;
                ++index;
                ++entry;
            } while (index < properties[0]);
        }
    }

    if (properties[1] != 0 &&
        acceptedCount < CSF_AT(uint32_t, self, 0x10))
    {
        unsigned int index = 0;
        unsigned int* entry = properties + 10;

        do
        {
            if (acceptedCount >= CSF_AT(uint32_t, self, 0x10))
                break;

            void* tableEntry = reinterpret_cast<void*>(
                GfxProps_ReadPropertyTableEntry(static_cast<int>(*entry)));
            const bool enabled =
                reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, tableEntry, 0)[0x30 / 4])(
                        tableEntry, nullptr);

            if (enabled &&
                reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, tableEntry, 0)[0x1C / 4])(
                        tableEntry, nullptr) == 3)
            {
                ++acceptedCount;
            }
            else
            {
                *entry = 0;
            }

            ++index;
            ++entry;
        } while (index < properties[1]);

        if (index < properties[1])
        {
            entry = properties + index + 10;
            do
            {
                *entry = 0;
                ++index;
                ++entry;
            } while (index < properties[1]);
        }
    }

    return static_cast<char>(GfxLights_ApplyFilteredLightProperties(properties));
}

HOOK(0x006D17B0, GfxLights_FilterLightPropertiesAndApplyTint, "auto");
