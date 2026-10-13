#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, SceneLightsObjects_GrowObjectStorage, 0x004F08D0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));

CSF_GLOBAL(const char*, g_pszKey_SCENEOBJS, 0x008A9C58);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9C5C, 0x008A9C5C);
CSF_GLOBAL(const char*, g_pszKey_ANIMACION, 0x008A9C60);
CSF_GLOBAL(const char*, g_pszKey_TIPO_OFFSET, 0x008A9C64);
CSF_GLOBAL(const char*, g_pszKey_OFFSET_8A9C68, 0x008A9C68);

static char __fastcall C_0083ef04_ReadSceneObjects(
    C_0083ef04* self, void* /*edx*/, S_00692b80* node)
{
    auto* property = reinterpret_cast<S_00692b80*>(
        FindProperty(reinterpret_cast<std::uint8_t*>(node), nullptr,
                     const_cast<char*>(g_pszKey_SCENEOBJS)));
    if (!property)
        return 0;

    (void)CSF_VTBL(0x00692B80);
    const unsigned int childCount = S_00692b80_GetChildCount(property, nullptr);
    if (childCount)
    {
        void* storage = reinterpret_cast<std::uint8_t*>(self) + 0x4;
        if (childCount > self->field_0008)
        {
            SceneLightsObjects_GrowObjectStorage(
                storage, nullptr,
                static_cast<int>(childCount - self->field_0008));
        }

        // Keep the text buffer immediately before the temporary string.
        // The original scans beyond 512 bytes if ReadText leaves it unterminated.
        alignas(8) std::uint8_t frameBytes[0x230];
        char* buffer = reinterpret_cast<char*>(frameBytes);
        std::uint8_t* nameBytes = frameBytes + 0x200;
        C_0082d410& name = *reinterpret_cast<C_0082d410*>(nameBytes);

        unsigned int index = 0;
        do
        {
            auto* component = S_00692b80_GetComponent(property, nullptr, index);
            auto* attributes = reinterpret_cast<std::uint8_t*>(component);

            CSF_AT(std::uint32_t, nameBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, nameBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, &name.field_0004, 0) = 0;
            CSF_AT(int, nameBytes, 0x1C) = -1;
            CSF_AT(int, nameBytes, 0x20) = 0;
            CSF_AT(float, nameBytes, 0x24) = 0.0f;

            MenuExtras_ReadText(attributes, nullptr,
                               const_cast<char*>(g_pszKey_ID_8A9C5C),
                               buffer, 512);
            AssignStringRange_429B60(
                &name, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));

            CSF_AT(int, nameBytes, 0x1C) =
                ReadIntegerAttribute(attributes, nullptr,
                                     const_cast<char*>(g_pszKey_ANIMACION));
            CSF_AT(int, nameBytes, 0x20) =
                ReadIntegerAttribute(attributes, nullptr,
                                     const_cast<char*>(g_pszKey_TIPO_OFFSET));
            CSF_AT(float, nameBytes, 0x24) =
                static_cast<float>(ReadFloatAttribute(
                    attributes, nullptr,
                    const_cast<char*>(g_pszKey_OFFSET_8A9C68)));

            if (self->field_000c >= self->field_0008)
            {
                SceneLightsObjects_GrowObjectStorage(
                    storage, nullptr, CSF_AT(int, self, 0x10));
            }

            const std::uint32_t entryIndex = self->field_000c;
            auto* entry = reinterpret_cast<std::uint8_t*>(self->field_0004)
                        + 40 * entryIndex;
            self->field_000c = entryIndex + 1;

            AssignStringSubstring(entry, nullptr, &name, 0, -1);
            CSF_AT(int, entry, 0x1C) = CSF_AT(int, nameBytes, 0x1C);
            CSF_AT(int, entry, 0x20) = CSF_AT(int, nameBytes, 0x20);
            CSF_AT(float, entry, 0x24) = CSF_AT(float, nameBytes, 0x24);

            if (CSF_AT(std::uint32_t, nameBytes, 0x18) >= 0x10)
            {
                if (name.field_0004)
                    Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));
            }

            ++index;
            CSF_AT(std::uint32_t, nameBytes, 0x18) = 15;
            CSF_AT(std::uint32_t, nameBytes, 0x14) = 0;
            CSF_AT(std::uint8_t, &name.field_0004, 0) = 0;
        }
        while (index < childCount);
    }
    return 1;
}

HOOK(0x004F09F0, C_0083ef04_ReadSceneObjects, "auto");
