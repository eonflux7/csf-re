#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_SpawnPointTempId, 0x008BC188);

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_743A10, 0x00743A10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, RegisterSpawnId, 0x007A6B60, (void* self, void* edx, int a2));

// Sets the excluded flag byte (+12) on the spawn-id entry registered for the
// player object a1 (its id lives at +44). The stack-held temp object is a
// 0xC-byte buffer that the original passes as `this` to the SpawnPoints helpers.
static void NetSpawnMessages_SetPlayerExcluded(int a1, char a2)
{
    alignas(8) uint8_t spawnPointTempBytes[0xC];
    void* tempEntry = spawnPointTempBytes;

    SpawnPoints_ApplyTempId(tempEntry, nullptr, static_cast<int>(g_SpawnPointTempId));

    uint8_t* playerObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));
    uint32_t playerSpawnField = CSF_AT(uint32_t, playerObject, 0x2C);

    uint32_t spawnIdArg;
    if ((playerSpawnField & 0x3FFFFF) != 0)
    {
        spawnIdArg = playerSpawnField;
    }
    else
    {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        spawnIdArg = 0;
    }

    char tempValid = sub_743A10(tempEntry, nullptr);
    uint32_t spawnOwnerId = tempValid ? CSF_AT(uint32_t, tempEntry, 0) : 0;

    uint32_t* spawnEntry = RegisterSpawnId(
        reinterpret_cast<void*>(static_cast<uintptr_t>(spawnOwnerId)),
        nullptr,
        static_cast<int>(spawnIdArg));
    if (spawnEntry)
        CSF_AT(uint8_t, spawnEntry, 0xC) = static_cast<uint8_t>(a2);

    SpawnPoints_ReleaseEntry(tempEntry, nullptr);
}

HOOK(0x743DE0, NetSpawnMessages_SetPlayerExcluded, "auto");
