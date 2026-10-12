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

static char __fastcall C_00858180_AddFireDispersionEntry(
    C_00858180* self, void* /*edx*/,
    char value1, char value2, char value3, char value4,
    char value5, char value6, char value7, char byteValue, char value8)
{
    alignas(8) std::uint8_t temporaryStringBytes[0x4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00867D40, "DisparaDispersion")));

    alignas(8) std::uint8_t hashBytes[0x4];
    CSF_AT(int, hashBytes, 0) =
        HashTemporaryString(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(hashBytes));

    GameConfig_WriteDwordField(entry, nullptr, value1);
    GameConfig_WriteDwordField(entry, nullptr, value2);
    GameConfig_WriteDwordField(entry, nullptr, value3);
    GameConfig_WriteDwordField(entry, nullptr, value4);
    GameConfig_WriteDwordField(entry, nullptr, value5);
    GameConfig_WriteDwordField(entry, nullptr, value6);
    GameConfig_WriteDwordField(entry, nullptr, value7);
    GameConfig_WriteByteField(entry, nullptr, byteValue);
    GameConfig_WriteDwordField(entry, nullptr, value8);

    return AddConfigEntry(self, nullptr, 1, entry);
}

HOOK(0x00744F70, C_00858180_AddFireDispersionEntry, "auto");
