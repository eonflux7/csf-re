#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, WriteConfigPair, 0x00656480, (void* self, void* edx, char a2, int a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

// Writes the "NotificaHayQueMigrar" config entry: hashes the key, finds or creates
// the entry, writes three key/value pairs, then adds the entry with flag 1.
static char __fastcall C_00858180_WriteMigrationNotifyEntry(C_00858180* self, void* /*edx*/,
                                                            char a2, int a3,
                                                            char a4, int a5,
                                                            char a6, int a7)
{
    // Temporary string object lives in the frame at [ebp-0x10], 12 bytes up to [ebp-0x4].
    alignas(8) uint8_t tempBytes[12];
    CSF_AT(uint32_t, tempBytes, 0) = reinterpret_cast<uint32_t>(self);
    InitializeTemporaryString(tempBytes, nullptr,
                              reinterpret_cast<std::int32_t>(CSF_STR(0x86847C, "NotificaHayQueMigrar")));

    // Hash result is stored in the local at [ebp-4]; FindOrCreateConfigEntry receives
    // the address of that local (lea eax,[ebp-4] / push eax), not the value.
    int hashKey = HashTemporaryString(CSF_AT(std::uint32_t, tempBytes, 0));

    void* configEntry = FindOrCreateConfigEntry(self, nullptr,
                                                static_cast<int>(reinterpret_cast<std::intptr_t>(&hashKey)));
    WriteConfigPair(configEntry, nullptr, a2, a3);
    WriteConfigPair(configEntry, nullptr, a4, a5);
    WriteConfigPair(configEntry, nullptr, a6, a7);
    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074CCA0, C_00858180_WriteMigrationNotifyEntry, "auto");
