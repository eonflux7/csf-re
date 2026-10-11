#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __fastcall, GameConfig_DestroyEmbeddedSettings, 0x006A24B0, (void* self, void* edx));

static int __fastcall GameConfig_DestroyMembers(void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    GameConfig_DestroyEmbeddedSettings(bytes + 0x1D94, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0x18F0, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0x13D8, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0xF9C, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0xB60, nullptr);

    if (CSF_AT(std::uint32_t, self, 0xB5C) >= 0x10u &&
        CSF_AT(void*, self, 0xB48))
    {
        Mem_FreeDebug(CSF_AT(void*, self, 0xB48));
    }
    CSF_AT(std::uint32_t, self, 0xB5C) = 15;
    CSF_AT(std::uint32_t, self, 0xB58) = 0;
    CSF_AT(std::uint8_t, self, 0xB48) = 0;

    GameConfig_DestroyEmbeddedSettings(bytes + 0x4B8, nullptr);

    if (CSF_AT(int*, self, 0x4AC))
    {
        MissionScript_DestroyElementRange(
            CSF_AT(int*, self, 0x4AC),
            CSF_AT(int*, self, 0x4B0));
        if (CSF_AT(void*, self, 0x4AC))
            Mem_FreeDebug(CSF_AT(void*, self, 0x4AC));
    }
    CSF_AT(std::uint32_t, self, 0x4AC) = 0;
    CSF_AT(std::uint32_t, self, 0x4B0) = 0;
    CSF_AT(std::uint32_t, self, 0x4B4) = 0;

    GameConfig_DestroyEmbeddedSettings(bytes + 0x68, nullptr);

    if (CSF_AT(std::uint32_t, self, 0x44) >= 0x10u &&
        CSF_AT(void*, self, 0x30))
    {
        Mem_FreeDebug(CSF_AT(void*, self, 0x30));
    }
    CSF_AT(std::uint32_t, self, 0x44) = 15;
    CSF_AT(std::uint32_t, self, 0x40) = 0;
    CSF_AT(std::uint8_t, self, 0x30) = 0;

    if (CSF_AT(std::uint32_t, self, 0x28) >= 0x10u &&
        CSF_AT(void*, self, 0x14))
    {
        Mem_FreeDebug(CSF_AT(void*, self, 0x14));
    }
    CSF_AT(std::uint32_t, self, 0x28) = 15;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint8_t, self, 0x14) = 0;

    int result = 0;
    if (CSF_AT(void*, self, 0x4))
        result = Mem_FreeDebug(CSF_AT(void*, self, 0x4));

    CSF_AT(std::uint32_t, self, 0x4) = 0;
    CSF_AT(std::uint32_t, self, 0x8) = 0;
    CSF_AT(std::uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x00756AC0, GameConfig_DestroyMembers, "auto");
