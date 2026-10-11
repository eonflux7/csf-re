#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, sub_50E9D0, 0x0050E9D0, ());
CSF_FUNC(void**, __fastcall, sub_50F230, 0x0050F230, (void* self, void* edx, void** a2, int* a3, int* a4));
CSF_FUNC(int, __fastcall, sub_519EC0, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void**, __fastcall, sub_755A30, 0x00755A30, (void* self, void* edx, void** a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, sub_755E90, 0x00755E90, (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_756320, 0x00756320, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(const char*, g_pszKey_MATERIALES, 0x008A892C);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_TIPO, 0x008A8930);
CSF_GLOBAL(const char*, g_pszKey_FLAGS, 0x008A8934);
CSF_GLOBAL(const char*, g_pszKey_FLAG, 0x008A8938);
CSF_GLOBAL(const char*, g_pszKey_ID_game_config, 0x008A893C);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_IMPACTO, 0x008A8940);
CSF_GLOBAL(const char*, g_pszKey_TEXTURA_DECALS, 0x008A8944);
CSF_GLOBAL(const char*, g_pszKey_EFECTO_IMPACTO, 0x008A8948);
CSF_GLOBAL(const char*, g_pszKey_SONIDO_PISADA, 0x008A894C);
CSF_GLOBAL(const char*, g_pszKey_COLOR_game_config, 0x008A8950);

static char __fastcall C_00869cd0_LoadMaterials(
    C_00869cd0* self, void* /*edx*/, unsigned char* node, int version)
{
    char text[256];
    char flagName[64];
    uint32_t insertionResult[2] = {};
    void* foundNode = nullptr;
    alignas(4) uint8_t material[0x50];
    alignas(4) uint8_t seenMaterials[0xC] = {};

    CSF_AT(uint32_t, seenMaterials, 0x04) =
        static_cast<uint32_t>(sub_50E9D0());
    CSF_AT(uint8_t, CSF_AT(void*, seenMaterials, 0x04), 0x11) = 1;
    CSF_AT(uint32_t, CSF_AT(void*, seenMaterials, 0x04), 0x04) =
        CSF_AT(uint32_t, seenMaterials, 0x04);
    CSF_AT(uint32_t, CSF_AT(void*, seenMaterials, 0x04), 0x00) =
        CSF_AT(uint32_t, seenMaterials, 0x04);
    CSF_AT(uint32_t, CSF_AT(void*, seenMaterials, 0x04), 0x08) =
        CSF_AT(uint32_t, seenMaterials, 0x04);
    CSF_AT(uint32_t, seenMaterials, 0x08) = 0;

    node = LookupNamedElement(node, nullptr, g_pszKey_MATERIALES);
    sub_755E90(reinterpret_cast<int>(material), 0);

    // These packet-listed "vtables" are the direct callee addresses.
    (void)CSF_VTBL(0x00692B80);
    const uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    for (uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<uint8_t*>(
            GetComponent(node, nullptr, index));

        MenuExtras_ReadText(component, nullptr,
                            const_cast<char*>(g_pszKey_NOMBRE_TIPO),
                            text, 256);
        AssignStringRange_429B60(material + 0x04, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));

        if (FindProperty(component, nullptr, const_cast<char*>(g_pszKey_FLAGS)))
        {
            CSF_AT(uint32_t, material, 0x20) =
                ReadIntegerAttribute(component, nullptr,
                                     const_cast<char*>(g_pszKey_FLAGS));
        }
        else
        {
            for (int bit = 0; bit < 8; ++bit)
            {
                _sprintf(flagName, CSF_STR(0x00869CE8, "%s%i"),
                         g_pszKey_FLAG, bit);
                if (FindProperty(component, nullptr, flagName))
                {
                    if (!ReadIntegerAttribute(component, nullptr, flagName))
                    {
                        CSF_AT(uint32_t, material, 0x20) &= ~(1u << bit);
                        continue;
                    }
                }
                CSF_AT(uint32_t, material, 0x20) |= 1u << bit;
            }

            if (version == 1)
            {
                if (CSF_AT(uint32_t, material, 0x20) & 4)
                    CSF_AT(uint32_t, material, 0x20) |= 0x40u;
                else
                    CSF_AT(uint32_t, material, 0x20) &= ~0x40u;
            }
            if (version <= 2)
            {
                if (CSF_AT(uint32_t, material, 0x20) & 0x40)
                    CSF_AT(uint32_t, material, 0x20) |= 0x80;
                else
                    CSF_AT(uint32_t, material, 0x20) &= 0xFFFFFF7F;
            }
        }

        CSF_AT(int, material, 0x00) =
            ReadIntegerAttribute(component, nullptr,
                                 const_cast<char*>(g_pszKey_ID_game_config));
        CSF_AT(int, material, 0x24) =
            ReadIntegerAttribute(component, nullptr,
                                 const_cast<char*>(g_pszKey_SONIDO_IMPACTO));
        CSF_AT(int, material, 0x28) =
            ReadIntegerAttribute(component, nullptr,
                                 const_cast<char*>(g_pszKey_EFECTO_IMPACTO));

        if (FindProperty(component, nullptr,
                         const_cast<char*>(g_pszKey_SONIDO_PISADA)))
            CSF_AT(int, material, 0x2C) =
                ReadIntegerAttribute(component, nullptr,
                                     const_cast<char*>(g_pszKey_SONIDO_PISADA));

        MenuExtras_ReadText(component, nullptr,
                            const_cast<char*>(g_pszKey_TEXTURA_DECALS),
                            text, 256);
        AssignStringRange_429B60(material + 0x30, nullptr, text,
                                static_cast<unsigned int>(std::strlen(text)));

        if (FindProperty(component, nullptr,
                         const_cast<char*>(g_pszKey_COLOR_game_config)))
            CSF_AT(int, material, 0x4C) =
                ReadIntegerAttribute(component, nullptr,
                                     const_cast<char*>(g_pszKey_COLOR_game_config));

        sub_756320(CSF_AT(void*, self, 0x04), nullptr,
                   reinterpret_cast<uint32_t*>(material));
        void** position = sub_755A30(
            seenMaterials, nullptr, &foundNode,
            reinterpret_cast<uint32_t*>(material));
        if (*position == CSF_AT(void*, seenMaterials, 0x04))
            sub_519EC0(seenMaterials, nullptr,
                       reinterpret_cast<int>(insertionResult),
                       reinterpret_cast<uint32_t*>(material));
    }

    if (CSF_AT(uint32_t, material, 0x48) >= 0x10 &&
        CSF_AT(void*, material, 0x34))
        Mem_FreeDebug(CSF_AT(void*, material, 0x34));
    CSF_AT(uint32_t, material, 0x48) = 15;
    CSF_AT(int, material, 0x44) = 0;
    CSF_AT(uint8_t, material, 0x34) = 0;

    if (CSF_AT(uint32_t, material, 0x1C) >= 0x10 &&
        CSF_AT(void*, material, 0x08))
        Mem_FreeDebug(CSF_AT(void*, material, 0x08));
    CSF_AT(uint32_t, material, 0x1C) = 15;
    CSF_AT(int, material, 0x18) = 0;
    CSF_AT(uint8_t, material, 0x08) = 0;

    (void)CSF_VTBL(0x0050F230);
    sub_50F230(seenMaterials, nullptr, reinterpret_cast<void**>(&node),
               CSF_AT(int*, CSF_AT(void*, seenMaterials, 0x04), 0x00),
               CSF_AT(int*, seenMaterials, 0x04));
    if (CSF_AT(void*, seenMaterials, 0x04))
        Mem_FreeDebug(CSF_AT(void*, seenMaterials, 0x04));
    return 1;
}

HOOK(0x00756420, C_00869cd0_LoadMaterials, "auto");
