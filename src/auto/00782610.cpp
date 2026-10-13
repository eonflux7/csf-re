#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, S_00692b80_ReadFloatAttribute, 0x00693990, (S_00692b80* self, void* /*edx*/, char* String1));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (S_00692b80* self, void* /*edx*/, char* attributeName));

CSF_GLOBAL(const char*, g_pszKey_FOG, 0x008A9C24);
CSF_GLOBAL(const char*, g_pszKey_VER, 0x008A9C28);
CSF_GLOBAL(const char*, g_pszKey_MIN, 0x008A9C2C);
CSF_GLOBAL(const char*, g_pszKey_MAX, 0x008A9C30);
CSF_GLOBAL(const char*, g_pszKey_DIS, 0x008A9C34);

// Reads the fog settings block (fog, version, min, max, distance) from an attribute source
// and writes them into the five-field output record.
static void __stdcall ReadFogSettingsFromAttributes(S_00692b80* self, void* outSettings)
{
    CSF_AT(float, outSettings, 0x00) =
        static_cast<float>(S_00692b80_ReadFloatAttribute(self, nullptr, const_cast<char*>(g_pszKey_FOG)));
    CSF_AT(float, outSettings, 0x04) =
        static_cast<float>(S_00692b80_ReadFloatAttribute(self, nullptr, const_cast<char*>(g_pszKey_VER)));
    CSF_AT(int, outSettings, 0x08) =
        ReadIntegerAttribute(self, nullptr, const_cast<char*>(g_pszKey_MIN));
    CSF_AT(int, outSettings, 0x0C) =
        ReadIntegerAttribute(self, nullptr, const_cast<char*>(g_pszKey_MAX));
    CSF_AT(float, outSettings, 0x10) =
        static_cast<float>(S_00692b80_ReadFloatAttribute(self, nullptr, const_cast<char*>(g_pszKey_DIS)));
}

HOOK(0x00782610, ReadFogSettingsFromAttributes, "auto");
