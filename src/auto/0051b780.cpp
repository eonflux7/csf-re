#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(char*, __cdecl, Hud_ExtractResourcePath, 0x00694960, (char* a1, uint8_t* a2, unsigned int a3));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(char, __fastcall, GameConfig_LoadVersionedSettings, 0x00756740, (C_00869cd0* self, void* , int pathArg));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_psz_Gfx_Textures, 0x00899D78);

static char __fastcall C_00840de4_LoadAndNormalizeResourceNames(
    C_00840de4* self, void* /*edx*/, int pathArg)
{
    const char loaded = GameConfig_LoadVersionedSettings(
        reinterpret_cast<C_00869cd0*>(self), nullptr, pathArg);
    if (!loaded)
        return 0;

    alignas(8) uint8_t resourcePath[0x100];
    alignas(8) char normalizedName[0x54];

    uint32_t index = 0;
    uint32_t recordOffset = 0;
    for (;; ++index, recordOffset += 80)
    {
        auto* records = CSF_AT(S_00756260*, self, 0x04);
        const uint32_t begin = records->field_0004;
        if (!begin)
            return loaded;

        const int32_t byteCount =
            static_cast<int32_t>(records->field_0008 - begin);
        if (index >= static_cast<uint32_t>(byteCount / 80))
            return loaded;

        auto* record = reinterpret_cast<uint8_t*>(
            records->field_0004 + recordOffset);
        void* resourceName = record + 0x30;

        if (CompareStringSubstring(
                resourceName, nullptr, 0,
                CSF_AT(uint32_t, record, 0x44),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0))
        {
            char* source;
            if (CSF_AT(uint32_t, resourceName, 0x18) < 0x10)
                source = reinterpret_cast<char*>(resourceName) + 0x04;
            else
                source = reinterpret_cast<char*>(
                    CSF_AT(uint32_t, resourceName, 0x04));

            Hud_ExtractResourcePath(source, resourcePath, 256);
            __stricmp(g_psz_Gfx_Textures,
                      reinterpret_cast<const char*>(resourcePath));

            if (CSF_AT(uint32_t, resourceName, 0x18) < 0x10)
                source = reinterpret_cast<char*>(resourceName) + 0x04;
            else
                source = reinterpret_cast<char*>(
                    CSF_AT(uint32_t, resourceName, 0x04));

            NormalizeResourceName(source, normalizedName, 80);
            AssignStringRange_429B60(
                resourceName, nullptr, normalizedName,
                static_cast<unsigned int>(std::strlen(normalizedName)));
        }
    }
}

HOOK(0x0051B780, C_00840de4_LoadAndNormalizeResourceNames, "auto");
