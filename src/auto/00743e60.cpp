#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_SpawnTempId, 0x008BC188);

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SpawnPoints_BeginTempEntry, 0x00743A10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, RegisterSpawnId, 0x007A6B60, (void* self, void* edx, int a2));

// Applies the temp spawn id to a temporary entry, validates the object's spawn id field,
// registers it, and reports whether the registered entry exists and is flagged.
static char __cdecl NetCallbacks_TryRegisterSpawnEntry(int a1)
{
    alignas(8) uint8_t entryBytes[0xC];

    SpawnPoints_ApplyTempId(entryBytes, nullptr, g_SpawnTempId);

    void* obj = reinterpret_cast<void*>(a1);
    int spawnIdArg;
    uint32_t fieldValue = CSF_AT(uint32_t, obj, 0x2C);
    if ((fieldValue & 0x3FFFFF) != 0)
    {
        spawnIdArg = static_cast<int>(fieldValue);
    }
    else
    {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        spawnIdArg = 0;
    }

    char beginOk = SpawnPoints_BeginTempEntry(entryBytes, nullptr);
    uint32_t entryFirstWord = CSF_AT(uint32_t, entryBytes, 0);
    uint32_t lookupKey = (beginOk != 0) ? entryFirstWord : 0u;

    uint32_t* registered = RegisterSpawnId(
        reinterpret_cast<void*>(static_cast<uintptr_t>(lookupKey)), nullptr, spawnIdArg);

    char succeeded = 0;
    if (registered != nullptr && CSF_AT(uint8_t, registered, 0xC) != 0)
    {
        succeeded = 1;
    }

    SpawnPoints_ReleaseEntry(entryBytes, nullptr);
    return succeeded;
}

HOOK(0x00743E60, NetCallbacks_TryRegisterSpawnEntry, "auto");
