#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, MenuExtras_InitializeFile, 0x00692B10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380, (void* self, void* edx));
CSF_FUNC(char, __fastcall, MenuExtras_LoadFile, 0x00694770, (void* self, void* edx, const char* path));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, ApplyConfigSettingsForVersion, 0x00756420, (void* self, void* edx, std::uint8_t* a2, int a3));

CSF_GLOBAL(const char*, g_pszKey_VERSION_game_config, 0x008A8928);

static char __fastcall GameConfig_LoadVersionedSettings(C_00869cd0* self, void* /*edx*/, int pathArg) {
    // Stack-resident settings file object (0x3C bytes), same as the original's [ebp-0x3C].
    alignas(4) std::uint8_t settingsFile[0x3C];

    MenuExtras_InitializeFile(settingsFile, nullptr);

    const char* path = reinterpret_cast<const char*>(static_cast<std::uintptr_t>(pathArg));
    if (MenuExtras_LoadFile(settingsFile, nullptr, path)) {
        std::uint8_t* settingsNode = CSF_AT(std::uint8_t*, settingsFile, 0);
        int versionValue = ReadIntegerAttribute(settingsNode, nullptr,
                                                const_cast<char*>(g_pszKey_VERSION_game_config));
        if (versionValue <= 3) {
            char result = ApplyConfigSettingsForVersion(self, nullptr, settingsNode, versionValue);
            MenuExtras_CloseFile(settingsFile, nullptr);
            return result;
        }
    }

    MenuExtras_CloseFile(settingsFile, nullptr);
    return 0;
}

HOOK(0x00756740, GameConfig_LoadVersionedSettings, "auto");
