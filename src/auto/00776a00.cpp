#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __fastcall, GameConfig_DestroyEmbeddedSettings, 0x006A24B0, (void* self, void*));

static int __fastcall GameConfig_ClearEmbeddedContainers(void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    if (CSF_AT(std::uint32_t, self, 0x1ED4) >= 0x10u &&
        CSF_AT(void*, self, 0x1EC0))
        Mem_FreeDebug(CSF_AT(void*, self, 0x1EC0));

    CSF_AT(std::uint32_t, self, 0x1ED4) = 15;
    CSF_AT(std::uint32_t, self, 0x1ED0) = 0;
    CSF_AT(std::uint8_t, self, 0x1EC0) = 0;

    GameConfig_DestroyEmbeddedSettings(bytes + 0x1A80, nullptr);

    if (CSF_AT(std::uint32_t, self, 0x1A7C) >= 0x10u &&
        CSF_AT(void*, self, 0x1A68))
        Mem_FreeDebug(CSF_AT(void*, self, 0x1A68));

    CSF_AT(std::uint32_t, self, 0x1A7C) = 15;
    CSF_AT(std::uint32_t, self, 0x1A78) = 0;
    CSF_AT(std::uint8_t, self, 0x1A68) = 0;

    if (CSF_AT(std::uint32_t, self, 0x1A60) >= 0x10u &&
        CSF_AT(void*, self, 0x1A4C))
        Mem_FreeDebug(CSF_AT(void*, self, 0x1A4C));

    CSF_AT(std::uint32_t, self, 0x1A60) = 15;
    CSF_AT(std::uint32_t, self, 0x1A5C) = 0;
    CSF_AT(std::uint8_t, self, 0x1A4C) = 0;

    GameConfig_DestroyEmbeddedSettings(bytes + 0x160C, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0x11D0, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0xD94, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0x924, nullptr);

    int* first = CSF_AT(int*, self, 0x910);
    if (first)
    {
        MissionScript_DestroyElementRange(first, CSF_AT(int*, self, 0x914));
        if (CSF_AT(void*, self, 0x910))
            Mem_FreeDebug(CSF_AT(void*, self, 0x910));
    }
    CSF_AT(std::uint32_t, self, 0x910) = 0;
    CSF_AT(std::uint32_t, self, 0x914) = 0;
    CSF_AT(std::uint32_t, self, 0x918) = 0;

    first = CSF_AT(int*, self, 0x8EC);
    if (first)
    {
        MissionScript_DestroyElementRange(first, CSF_AT(int*, self, 0x8F0));
        if (CSF_AT(void*, self, 0x8EC))
            Mem_FreeDebug(CSF_AT(void*, self, 0x8EC));
    }
    CSF_AT(std::uint32_t, self, 0x8EC) = 0;
    CSF_AT(std::uint32_t, self, 0x8F0) = 0;
    CSF_AT(std::uint32_t, self, 0x8F4) = 0;

    first = CSF_AT(int*, self, 0x8C8);
    if (first)
    {
        MissionScript_DestroyElementRange(first, CSF_AT(int*, self, 0x8CC));
        if (CSF_AT(void*, self, 0x8C8))
            Mem_FreeDebug(CSF_AT(void*, self, 0x8C8));
    }
    CSF_AT(std::uint32_t, self, 0x8C8) = 0;
    CSF_AT(std::uint32_t, self, 0x8CC) = 0;
    CSF_AT(std::uint32_t, self, 0x8D0) = 0;

    GameConfig_DestroyEmbeddedSettings(bytes + 0x464, nullptr);
    GameConfig_DestroyEmbeddedSettings(bytes + 0x18, nullptr);

    int result = 0;
    if (CSF_AT(void*, self, 0x4))
        result = Mem_FreeDebug(CSF_AT(void*, self, 0x4));

    CSF_AT(std::uint32_t, self, 0x4) = 0;
    CSF_AT(std::uint32_t, self, 0x8) = 0;
    CSF_AT(std::uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x00776A00, GameConfig_ClearEmbeddedContainers, "auto");
