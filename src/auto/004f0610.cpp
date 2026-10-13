#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLights_GrowStorage, 0x004F0510, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));

CSF_GLOBAL(float, g_fInv255, 0x00830A00);
CSF_GLOBAL(const char*, g_pszKey_LIGHTS, 0x008A9C38);
CSF_GLOBAL(const char*, g_pszKey_POS_8A9C44, 0x008A9C44);
CSF_GLOBAL(const char*, g_pszKey_COLOR_8A9C4C, 0x008A9C4C);
CSF_GLOBAL(const char*, g_pszKey_RADIO_8A9C50, 0x008A9C50);
CSF_GLOBAL(const char*, g_pszKey_MODULATE, 0x008A9C54);

// Retain the vtable binding listed in the packet.
[[maybe_unused]] static const auto g_DataNodeVtable = CSF_VTBL(0x00692B80);

static char __fastcall C_0083eef4_LoadLights(
    C_0083eef4* self, void* /*edx*/, S_00692b80* node)
{
    auto* lights = reinterpret_cast<S_00692b80*>(
        FindProperty(reinterpret_cast<std::uint8_t*>(node), nullptr,
                     const_cast<char*>(g_pszKey_LIGHTS)));
    if (!lights)
        return 0;

    const unsigned int childCount =
        static_cast<unsigned int>(S_00692b80_GetChildCount(lights, nullptr));

    if (childCount != 0)
    {
        void* storage = reinterpret_cast<std::uint8_t*>(self) + 4;
        const std::uint32_t capacity = self->field_0008;
        if (childCount > capacity)
            SceneLights_GrowStorage(
                storage, nullptr, static_cast<int>(childCount - capacity));

        for (unsigned int index = 0; index < childCount; ++index)
        {
            auto* component = S_00692b80_GetComponent(lights, nullptr, index);
            auto* componentBytes = reinterpret_cast<std::uint8_t*>(component);

            // Reserve the complete original frame-sized region. Unspecified
            // record padding is initialized to zero rather than stack garbage.
            alignas(8) std::uint8_t lightBytes[0x30] = {};
            CSF_AT(float, lightBytes, 0x0C) = 1.0f;

            DataNode_ReadVector3Property(
                component, nullptr, const_cast<char*>(g_pszKey_POS_8A9C44),
                reinterpret_cast<int>(lightBytes + 0x10));

            CSF_AT(float, lightBytes, 0x1C) = static_cast<float>(
                ReadFloatAttribute(componentBytes, nullptr,
                                   const_cast<char*>(g_pszKey_RADIO_8A9C50)));

            const std::uint32_t color = static_cast<std::uint32_t>(
                ReadIntegerAttribute(componentBytes, nullptr,
                                     const_cast<char*>(g_pszKey_COLOR_8A9C4C)));

            CSF_AT(float, lightBytes, 0x00) =
                static_cast<float>(static_cast<std::uint8_t>(color)) * g_fInv255;
            CSF_AT(float, lightBytes, 0x0C) = 1.0f;
            CSF_AT(float, lightBytes, 0x04) =
                static_cast<float>(static_cast<std::uint8_t>(color >> 8)) * g_fInv255;
            CSF_AT(float, lightBytes, 0x08) =
                static_cast<float>(static_cast<std::uint8_t>(color >> 16)) * g_fInv255;

            const int modulate = ReadIntegerAttribute(
                componentBytes, nullptr, const_cast<char*>(g_pszKey_MODULATE));
            CSF_AT(std::uint8_t, lightBytes, 0x20) = modulate != 0;

            if (self->field_000c >= self->field_0008)
                SceneLights_GrowStorage(
                    storage, nullptr,
                    static_cast<int>(CSF_AT(std::uint32_t, self, 0x10)));

            const std::uint32_t count = self->field_000c;
            auto* destination = reinterpret_cast<std::uint8_t*>(self->field_0004)
                              + 36 * count;
            self->field_000c = count + 1;
            std::memcpy(destination, lightBytes, 0x24);
        }
    }

    return 1;
}

HOOK(0x004F0610, C_0083eef4_LoadLights, "auto");
