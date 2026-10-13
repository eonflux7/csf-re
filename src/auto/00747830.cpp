#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

// Launches a "Detona" (detonate) config request for this spawn point: hashes the
// temporary string object, finds or creates the matching config entry, writes the six
// dword arguments into it, then registers the entry with type 1.
static char __fastcall SpawnPoint_LaunchSpawnRequest(C_00858180* self, void* /*edx*/,
                                                     char a2, char a3, char a4,
                                                     char a5, char a6, char a7)
{
    // Temporary string object: its first dword is the owner pointer (vtable slot in the original).
    alignas(8) std::uint8_t tempObjectBytes[8];
    *reinterpret_cast<void**>(tempObjectBytes) = self;
    InitializeTemporaryString(tempObjectBytes, nullptr,
                              reinterpret_cast<std::int32_t>(CSF_STR(0x867F30, "Detona")));

    int hashKey = HashTemporaryString(*reinterpret_cast<std::uint32_t*>(tempObjectBytes));

    C_008622e8* configEntry = reinterpret_cast<C_008622e8*>(
        FindOrCreateConfigEntry(self, nullptr,
                                static_cast<int>(reinterpret_cast<std::uintptr_t>(&hashKey))));

    GameConfig_WriteDwordField(configEntry, nullptr, a2);
    GameConfig_WriteDwordField(configEntry, nullptr, a3);
    GameConfig_WriteDwordField(configEntry, nullptr, a4);
    GameConfig_WriteDwordField(configEntry, nullptr, a5);
    GameConfig_WriteDwordField(configEntry, nullptr, a6);
    GameConfig_WriteDwordField(configEntry, nullptr, a7);

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x747830, SpawnPoint_LaunchSpawnRequest, "auto");
