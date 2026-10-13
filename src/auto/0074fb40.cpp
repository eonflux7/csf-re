#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall C_00858180_AddEightValueConfigEntry(
    C_00858180* self, void* /*edx*/,
    char value1, char value2, char value3, char value4,
    char value5, char value6, char value7, char value8)
{
    alignas(8) std::uint8_t temporaryStringBytes[4];
    CSF_AT(std::uint32_t, temporaryStringBytes, 0) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self));

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(CSF_ADDR_DATA(0x008679EC))));

    int entryKey = HashTemporaryString(
        CSF_AT(std::uint32_t, temporaryStringBytes, 0));

    void* entry = FindOrCreateConfigEntry(
        self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&entryKey)));

    GameConfig_WriteDwordField(entry, nullptr, value1);
    GameConfig_WriteDwordField(entry, nullptr, value2);
    GameConfig_WriteDwordField(entry, nullptr, value3);
    GameConfig_WriteDwordField(entry, nullptr, value4);
    GameConfig_WriteDwordField(entry, nullptr, value5);
    GameConfig_WriteDwordField(entry, nullptr, value6);
    GameConfig_WriteDwordField(entry, nullptr, value7);
    GameConfig_WriteDwordField(entry, nullptr, value8);

    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x0074FB40, C_00858180_AddEightValueConfigEntry, "auto");
