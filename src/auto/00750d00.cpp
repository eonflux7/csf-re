#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_HandleSingleplayerSpawnRequest(
    C_00858180* self, void* /*edx*/,
    char field1, char field2, char field3, char field4,
    char field5, char field6, char wordField)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        reinterpret_cast<std::uint32_t>(self);

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868944, "MensajeFliEx")));

    alignas(8) std::uint8_t keyBytes[4];
    CSF_AT(int, keyBytes, 0) =
        HashTemporaryString(CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr, reinterpret_cast<int>(keyBytes));

    GameConfig_WriteDwordField(entry, nullptr, field1);
    GameConfig_WriteDwordField(entry, nullptr, field2);
    GameConfig_WriteDwordField(entry, nullptr, field3);
    GameConfig_WriteDwordField(entry, nullptr, field4);
    GameConfig_WriteDwordField(entry, nullptr, field5);
    GameConfig_WriteDwordField(entry, nullptr, field6);
    GameConfig_WriteWordField(entry, nullptr, wordField);
    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x00750D00, C_00858180_HandleSingleplayerSpawnRequest, "auto");
