#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_WriteRespawnFields(
    C_00858180* self, void* /*edx*/, char byteValue, char dwordValue)
{
    alignas(8) std::uint8_t tempStringBytes[0xC];
    CSF_AT(void*, tempStringBytes, 0) = self;

    InitializeTemporaryString(
        tempStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00867DA0, "Respawn")));

    int entryId = HashTemporaryString(CSF_AT(std::uint32_t, tempStringBytes, 0));
    void* configEntry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(&entryId));

    GameConfig_WriteByteField(configEntry, nullptr, byteValue);
    GameConfig_WriteDwordField(configEntry, nullptr, dwordValue);
    return AddConfigEntry(self, nullptr, 0, configEntry);
}

HOOK(0x007453D0, C_00858180_WriteRespawnFields, "auto");
