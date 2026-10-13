#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(C_0082d410*, __fastcall, C_0082d410_ConstructFromText, 0x00434150, (C_0082d410* self, void* , uint32_t* a2));
CSF_FUNC(int, __cdecl, FxParticleManager_HasNamedEffect, 0x00437D60, (int a1, const char* a2));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_69B700, 0x0069B700, ());
CSF_FUNC(int, __fastcall, MenuScenarioSelect_GetScenarioMask, 0x0069BC50, (void* self, void* , char* text));
CSF_FUNC(char, __stdcall, CGuiBuilder_ReadFourComponents, 0x0071DA70, (std::uint8_t* node, float* output));
CSF_FUNC(int, __fastcall, CGuiBuilder_GetFont, 0x0071DB60, (CGuiBuilder* self, void* , std::uint8_t* properties));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszKey_CADENA_CHAR, 0x008A8624);
CSF_GLOBAL(const char*, g_pszKey_COLOR, 0x008A8628);
CSF_GLOBAL(const char*, g_pszKey_VERTICAL, 0x008A867C);
CSF_GLOBAL(const char*, g_pszKey_ALINEAMIENTO, 0x008A86C4);
CSF_GLOBAL(const char*, g_pszKey_HORIZONTAL, 0x008A86C8);
CSF_GLOBAL(const char*, g_pszKey_CADENA_FLI, 0x008A86E4);
CSF_GLOBAL(const char*, g_pszKey_CADENA_FILE, 0x008A86E8);
CSF_GLOBAL(const char*, g_pszKey_MAPPED, 0x008A8750);
CSF_GLOBAL(const char*, g_pszKey_FIXED_COLORFONT, 0x008A8788);

static int __fastcall CGuiBuilder_CreateText(
    CGuiBuilder* self, void* /*edx*/, S_00692b80* properties)
{
    auto* node = reinterpret_cast<std::uint8_t*>(properties);
    alignas(8) char fileText[0x100];
    alignas(8) char text[0x100];
    alignas(8) char textId[0xC];
    CSF_AT(std::uint32_t, textId, 0) = 0;

    int font = CGuiBuilder_GetFont(self, nullptr, node);
    std::memset(text, 0, sizeof(text));
    std::memset(fileText, 0, sizeof(fileText));

    void* factory = reinterpret_cast<void*>(self->field_0004);
    void* control;
    if (font)
    {
        control = reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, factory, 0)[0x54 / 4])(factory, nullptr, font);
        ReleaseObjectReference(reinterpret_cast<void*>(font), nullptr);
    }
    else
    {
        control = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, factory, 0)[0x58 / 4])(factory, nullptr);
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_CADENA_CHAR)))
    {
        MenuExtras_ReadText(node, nullptr,
            const_cast<char*>(g_pszKey_CADENA_CHAR), text, 256);
        reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, control, 0)[0x3C / 4])(control, nullptr, text);
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_CADENA_FLI)) &&
        FindProperty(node, nullptr, const_cast<char*>(g_pszKey_CADENA_FILE)))
    {
        MenuExtras_ReadText(node, nullptr,
            const_cast<char*>(g_pszKey_CADENA_FILE), fileText, 256);
        MenuExtras_ReadText(node, nullptr,
            const_cast<char*>(g_pszKey_CADENA_FLI), textId, 5);

        // The string extends into the adjacent 0x14-byte scratch region.
        alignas(8) std::uint8_t idStringBytes[0xC + 0x14];
        auto& idString = *reinterpret_cast<C_0082d410*>(idStringBytes);
        CSF_AT(std::uint32_t, idStringBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, idStringBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&idString.field_0004)[0] = 0;
        AssignStringRange_429B60(&idString, nullptr, textId,
            static_cast<unsigned int>(std::strlen(textId)));

        bool valid = false;
        if (CompareStringSubstring(&idString, nullptr, 0,
                CSF_AT(std::uint32_t, idStringBytes, 0x14),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0) != 0)
        {
            alignas(8) std::uint8_t fileStringBytes[0x1C];
            auto& fileString = *reinterpret_cast<C_0082d410*>(fileStringBytes);
            auto* constructed = C_0082d410_ConstructFromText(
                &fileString, nullptr, reinterpret_cast<std::uint32_t*>(fileText));
            valid = static_cast<std::uint8_t>(FxParticleManager_HasNamedEffect(
                reinterpret_cast<int>(constructed),
                reinterpret_cast<const char*>(&g_byte_82E65C))) != 0;
            CMenuSaveGame_ResetStringStorage(&fileString, nullptr);
        }
        CMenuSaveGame_ResetStringStorage(&idString, nullptr);

        if (valid)
        {
            int32_t manager = GetGlobalValue_69B700();
            int scenarioMask = MenuScenarioSelect_GetScenarioMask(
                reinterpret_cast<void*>(manager), nullptr, fileText);
            if (scenarioMask)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, char*, int)>(
                    CSF_AT(void**, control, 0)[0x2C / 4])(
                        control, nullptr, textId, scenarioMask);
            }
        }
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_FIXED_COLORFONT)))
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint8_t*)>(
            CSF_AT(void**, control, 0)[0x60 / 4])(
                control, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x50);
    }
    else
    {
        alignas(8) std::uint8_t colorBytes[0x14];
        auto* color = reinterpret_cast<float*>(colorBytes);
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        color[3] = 1.0f;
        auto* colorProperty = FindProperty(
            node, nullptr, const_cast<char*>(g_pszKey_COLOR));
        if (CGuiBuilder_ReadFourComponents(colorProperty, color))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
                CSF_AT(void**, control, 0)[0x60 / 4])(control, nullptr, color);
        }
    }

    auto* alignment = FindProperty(
        node, nullptr, const_cast<char*>(g_pszKey_ALINEAMIENTO));
    if (!alignment)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 1);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr, 1);
    }
    else
    {
        auto* horizontal = FindProperty(
            alignment, nullptr, const_cast<char*>(g_pszKey_HORIZONTAL));
        if (horizontal)
        {
            CopyAttributeText(horizontal, nullptr, text, 256);
            if (!__stricmp(text, CSF_STR(0x00864BB4, "RIGHT")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 2);
            }
            else if (!__stricmp(text, CSF_STR(0x00864BAC, "CENTER")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 1);
            }
            else if (!__stricmp(text, CSF_STR(0x00864BA4, "LEFT")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x68 / 4])(control, nullptr, 0);
            }
        }

        auto* vertical = FindProperty(
            alignment, nullptr, const_cast<char*>(g_pszKey_VERTICAL));
        if (vertical)
        {
            CopyAttributeText(vertical, nullptr, text, 256);
            if (!__stricmp(text, CSF_STR(0x00864B9C, "BOTTOM")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr, 2);
            }
            else if (!__stricmp(text, CSF_STR(0x00864BAC, "CENTER")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr, 1);
            }
            else if (!__stricmp(text, CSF_STR(0x00864B98, "TOP")))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, control, 0)[0x6C / 4])(control, nullptr, 0);
            }
        }
    }

    if (FindProperty(node, nullptr, const_cast<char*>(g_pszKey_MAPPED)))
    {
        void** table = CSF_AT(void**, control, 0);
        std::uint32_t argument = 1u - static_cast<std::uint32_t>(
            ReadIntegerProperty(node, nullptr, const_cast<char*>(g_pszKey_MAPPED)));
        argument = (argument & 0xFFFFFF00u) | (argument == 0 ? 1u : 0u);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            table[0xA8 / 4])(control, nullptr, static_cast<int>(argument));
    }

    return reinterpret_cast<int>(control);
}

HOOK(0x0071F910, CGuiBuilder_CreateText, "auto");
