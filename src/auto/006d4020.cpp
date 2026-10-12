#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(bool, __cdecl, CoreFiles_CheckPageAvailabilityWithoutOption, 0x00698BA0, (const char* name, char mode));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(std::uint32_t, g_ParticleReadParam, 0x0086103C);
CSF_GLOBAL(char, g_PngExtensionTerminator, 0x00861040);
CSF_GLOBAL(char, g_ExtensionReplacementBase, 0x008BB4FC);
CSF_GLOBAL(char, g_AlternateTexturePath, 0x008BB500);

static std::uint32_t* __cdecl GfxClumpDict_DiscoverAlternateTextures(
    std::uint32_t* clump, void* options)
{
    if (!clump)
        return clump;

    void* entry = reinterpret_cast<void*>(*clump);
    if (!entry)
        return clump;

    if (CSF_AT(std::uint32_t, entry, 0x54) == 1)
        CSF_AT(std::uint32_t, entry, 0x54) = 2;

    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

    const char* baseName =
        reinterpret_cast<const char*>(entry) + 0x10;
    alignas(8) uint8_t alternateNameBytes[0x108];
    char* alternateName = reinterpret_cast<char*>(alternateNameBytes);
    int variantIndex = 1;

    for (;;)
    {
        _sprintf(alternateName, CSF_STR(0x00861050, "%s_Alt%.3d"),
                 baseName, variantIndex);

        if (GlobalInt_FindNameId(alternateNameBytes) != 0)
        {
            ++variantIndex;
            continue;
        }

        if (!CSF_AT(uint8_t, options, 0x8))
            break;

        _sprintf(&g_AlternateTexturePath,
                 CSF_STR(0x00861044, "%s%s.dds"),
                 CSF_AT(const char*, options, 0x0), alternateName);

        bool available = CoreFiles_CheckPageAvailabilityWithoutOption(
            &g_AlternateTexturePath, 0);

        if (!available)
        {
            const std::size_t length = std::strlen(&g_AlternateTexturePath);
            const char terminator = g_PngExtensionTerminator;
            char* extension = &g_ExtensionReplacementBase + length;
            const std::uint32_t extensionText = g_ParticleReadParam;
            std::memcpy(extension, &extensionText, 4);
            extension[4] = terminator;

            available = CoreFiles_CheckPageAvailabilityWithoutOption(
                &g_AlternateTexturePath, 0);
        }

        if (!available)
            break;

        accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));

        void* textureManager =
            ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

        ((void(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, textureManager, 0)[0x1C / 4])(
                textureManager, nullptr, &g_AlternateTexturePath);

        ++variantIndex;
    }

    const std::uint32_t variantCount =
        static_cast<std::uint32_t>(variantIndex - 1);
    if (CSF_AT(std::uint32_t, options, 0x4) < variantCount)
        CSF_AT(std::uint32_t, options, 0x4) = variantCount;

    return clump;
}

HOOK(0x006D4020, GfxClumpDict_DiscoverAlternateTextures, "auto");
