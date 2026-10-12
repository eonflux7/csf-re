#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00860288_GetTintColorDwords, 0x006B40D0, (C_00860288* self, void* , uint32_t* outColor));
CSF_FUNC(int, __fastcall, GetBit2OfField4, 0x006B8170, (void* self, void*));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __cdecl, GfxSkin_GetDistanceOverrideOwner, 0x006B8840, (int a1));
CSF_FUNC(int, __cdecl, GfxProps_ReadPropertyTableEntry, 0x006B88C0, (int a1));
CSF_FUNC(unsigned int, __stdcall, GfxLights_CommitLightList, 0x006D1050, (unsigned int* a1));
CSF_FUNC(unsigned int, __fastcall, S_006d10c0_ApplyColor, 0x006D10C0, (void* self, void* edx, unsigned int* a2, char a3, int a4, int a5, int a6));
CSF_FUNC(void, __cdecl, GfxLights_GetObjectSecondaryColor, 0x006D1730, (int a1, float* a2));
CSF_FUNC(int, __fastcall, C_00861988_SetSecondaryParameters, 0x006FD040, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00861988_SetTertiaryParameters, 0x006FD080, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, GfxLights_SetParameterMode, 0x006FD0C0, (void* self, void* edx, char a2));

CSF_GLOBAL(float, g_ColorWhite, 0x008A7D90);
CSF_GLOBAL(float, g_ColorWhite_G, 0x008A7D94);
CSF_GLOBAL(float, g_ColorWhite_B, 0x008A7D98);
CSF_GLOBAL(float, g_ColorWhite_A, 0x008A7D9C);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);

// The packet also classifies the direct ApplyColor call target as a vtable.
static const auto kApplyColorTableReference = CSF_VTBL(0x006D10C0);

static int GfxLights_GetColorBits(float value)
{
    int bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static char __fastcall S_006d10c0_ConfigureObjectLights(
    S_006d10c0* self, void* /*edx*/,
    C_00860288* lights, C_0085ec18* object)
{
    if (lights->vtbl == nullptr && lights->field_0004 == 0)
        return 0;

    const int objectIndex = reinterpret_cast<int>(object);
    void* attachment = reinterpret_cast<void*>(
        GfxObjectProps_GetObjectAttachment(objectIndex));

    if (self->field_0019)
        return static_cast<char>(self->field_0019);

    alignas(8) uint8_t tintResultBytes[0x10];
    alignas(8) uint8_t secondaryBytes[0x20];
    alignas(8) uint8_t tertiaryBytes[0x18];
    auto* tintResult = reinterpret_cast<uint32_t*>(tintResultBytes);
    auto* secondary = reinterpret_cast<float*>(secondaryBytes);
    auto* tertiary = reinterpret_cast<float*>(tertiaryBytes);

    const float* baseColor = self->field_001a
        ? reinterpret_cast<const float*>(reinterpret_cast<uint8_t*>(self) + 0x1C)
        : &g_ColorBlack;
    tertiary[0] = baseColor[0];
    tertiary[1] = baseColor[1];
    tertiary[2] = baseColor[2];
    tertiary[3] = baseColor[3];

    const int overrideOwner = GfxSkin_GetDistanceOverrideOwner(objectIndex);
    if (overrideOwner)
    {
        auto* tintObject = CSF_AT(C_00860288*,
            reinterpret_cast<void*>(overrideOwner), 0x10);
        if (static_cast<uint8_t>(tintObject->vIsField0088Set()))
        {
            const auto* tint = reinterpret_cast<const float*>(
                C_00860288_GetTintColorDwords(tintObject, nullptr, tintResult));
            tertiary[0] += tint[0];
            tertiary[1] += tint[1];
            tertiary[2] += tint[2];
            tertiary[3] += tint[3];
        }
    }

    if (static_cast<uint8_t>(GetBit2OfField4(attachment, nullptr)))
    {
        const int whiteRed = GfxLights_GetColorBits(g_ColorWhite);
        const int whiteGreen = GfxLights_GetColorBits(g_ColorWhite_G);
        const int whiteBlue = GfxLights_GetColorBits(g_ColorWhite_B);
        const int whiteAlpha = GfxLights_GetColorBits(g_ColorWhite_A);
        S_006d10c0_ApplyColor(self, nullptr,
            reinterpret_cast<unsigned int*>(lights),
            static_cast<char>(whiteRed), whiteGreen, whiteBlue, whiteAlpha);
        return static_cast<char>(
            GfxLights_CommitLightList(reinterpret_cast<unsigned int*>(lights)));
    }

    std::memset(secondary, 0, 12);
    secondary[3] = 1.0f;
    GfxLights_GetObjectSecondaryColor(objectIndex, secondary);
    C_00861988_SetSecondaryParameters(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        reinterpret_cast<uint32_t*>(secondary));
    GfxLights_SetParameterMode(
        reinterpret_cast<void*>(self->field_0030), nullptr, 0);
    C_00861988_SetTertiaryParameters(
        reinterpret_cast<void*>(self->field_0030), nullptr,
        reinterpret_cast<uint32_t*>(tertiary));

    void* cachedProvider = GetCachedObjectProvider(
        CSF_AT(void*, self, 0x2C), nullptr);
    const uint32_t providerValue = reinterpret_cast<uint32_t>(cachedProvider);

    uint32_t acceptedCount = 0;
    uint32_t primaryIndex = 0;
    uint32_t* primaryEntry = &lights->field_0008;
    while (primaryIndex < reinterpret_cast<uintptr_t>(lights->vtbl))
    {
        if (acceptedCount >= CSF_AT(uint32_t, self, 0x14))
            break;

        const uint32_t entry = *primaryEntry;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(entry), 0x1) == 1)
        {
            ++acceptedCount;
            void* property = reinterpret_cast<void*>(
                GfxProps_ReadPropertyTableEntry(static_cast<int>(entry)));
            if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, property, 0)[0x1C / 4])(property, nullptr) == 1)
            {
                C_00861988_SetSecondaryParameters(
                    property, nullptr, reinterpret_cast<uint32_t*>(secondary));
            }
        }
        else if (entry == providerValue)
        {
            *primaryEntry = 0;
        }
        ++primaryIndex;
        ++primaryEntry;
    }

    while (primaryIndex < reinterpret_cast<uintptr_t>(lights->vtbl))
    {
        const uint32_t entry = *primaryEntry;
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(entry), 0x1) == 1 ||
            entry == providerValue)
        {
            *primaryEntry = 0;
        }
        ++primaryIndex;
        ++primaryEntry;
    }

    if (acceptedCount < CSF_AT(uint32_t, self, 0x10))
    {
        uint32_t secondaryIndex = 0;
        uint32_t* secondaryEntry = &lights->field_0028;
        while (secondaryIndex < lights->field_0004)
        {
            if (acceptedCount >= CSF_AT(uint32_t, self, 0x10))
                break;

            void* property = reinterpret_cast<void*>(
                GfxProps_ReadPropertyTableEntry(static_cast<int>(*secondaryEntry)));
            if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, property, 0)[0x30 / 4])(property, nullptr))
            {
                ++acceptedCount;
                if (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, property, 0)[0x1C / 4])(property, nullptr) == 1)
                {
                    C_00861988_SetSecondaryParameters(
                        property, nullptr, reinterpret_cast<uint32_t*>(secondary));
                }
            }
            else
            {
                *secondaryEntry = 0;
            }
            ++secondaryIndex;
            ++secondaryEntry;
        }

        while (secondaryIndex < lights->field_0004)
        {
            *secondaryEntry = 0;
            ++secondaryIndex;
            ++secondaryEntry;
        }
    }

    return static_cast<char>(
        GfxLights_CommitLightList(reinterpret_cast<unsigned int*>(lights)));
}

HOOK(0x006D19B0, S_006d10c0_ConfigureObjectLights, "auto");
