#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, WriteConfigPair, 0x00656480, (void* self, void* edx, char a2, int a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

static char __fastcall NetProyectilDDL_ApplySettingsB(
    C_00858180* self, void* /*edx*/,
    char byteField,
    char d0, char d1, char d2, char d3, char d4, char d5, char d6, char d7,
    char d8, char d9, char d10, char d11, char d12, char d13, char d14,
    char flag, char pairA, int pairB)
{
    // The original pushes `this` into a 4-byte stack slot, passes that slot as
    // ecx to InitializeTemporaryString (which overwrites it), then hashes its value.
    alignas(4) uint8_t tempBytes[4];
    CSF_AT(std::int32_t, tempBytes, 0) = reinterpret_cast<std::int32_t>(self);

    InitializeTemporaryString(tempBytes, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x008679E0, "Contacto")));
    int key = HashTemporaryString(static_cast<std::uint32_t>(CSF_AT(std::int32_t, tempBytes, 0)));

    void* entry = FindOrCreateConfigEntry(self, nullptr, reinterpret_cast<int>(&key));

    GameConfig_WriteByteField(entry, nullptr, byteField);
    GameConfig_WriteDwordField(entry, nullptr, d0);
    GameConfig_WriteDwordField(entry, nullptr, d1);
    GameConfig_WriteDwordField(entry, nullptr, d2);
    GameConfig_WriteDwordField(entry, nullptr, d3);
    GameConfig_WriteDwordField(entry, nullptr, d4);
    GameConfig_WriteDwordField(entry, nullptr, d5);
    GameConfig_WriteDwordField(entry, nullptr, d6);
    GameConfig_WriteDwordField(entry, nullptr, d7);
    GameConfig_WriteDwordField(entry, nullptr, d8);
    GameConfig_WriteDwordField(entry, nullptr, d9);
    GameConfig_WriteDwordField(entry, nullptr, d10);
    GameConfig_WriteDwordField(entry, nullptr, d11);
    GameConfig_WriteDwordField(entry, nullptr, d12);
    GameConfig_WriteDwordField(entry, nullptr, d13);
    GameConfig_WriteDwordField(entry, nullptr, d14);
    WriteConfigFlag(entry, nullptr, flag);
    WriteConfigPair(entry, nullptr, pairA, pairB);

    return AddConfigEntry(self, nullptr, 0, entry);
}

HOOK(0x007475F0, NetProyectilDDL_ApplySettingsB, "auto");
