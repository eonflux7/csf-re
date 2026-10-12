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

static char __fastcall C_00858180_AddPlayerConfigEntry(
    C_00858180* self, void* /*edx*/,
    char firstValue, char secondValue, char byteValue, char fourthValue)
{
    alignas(8) std::uint8_t temporaryStringBytes[12];
    CSF_AT(void*, temporaryStringBytes, 0) = self;

    InitializeTemporaryString(
        temporaryStringBytes, nullptr,
        static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(CSF_ADDR_DATA(0x008679EC))));

    int lookupKey = HashTemporaryString(
        CSF_AT(std::uint32_t, temporaryStringBytes, 0));
    void* entry = FindOrCreateConfigEntry(
        self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&lookupKey)));

    GameConfig_WriteDwordField(entry, nullptr, firstValue);
    GameConfig_WriteDwordField(entry, nullptr, secondValue);
    GameConfig_WriteByteField(entry, nullptr, byteValue);
    GameConfig_WriteDwordField(entry, nullptr, fourthValue);
    return AddConfigEntry(self, nullptr, 1, entry);
}

HOOK(0x007451C0, C_00858180_AddPlayerConfigEntry, "auto");
