// SpawnPoints_ActivateSpawn - reimplementation of 0x00742360 (NetCogibleDDL)
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

// Callee bindings (copied from packet bind lines where given)
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, sub_64AD60, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_64CEB0, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));
// cdecl, takes the temporary string object by value (its first dword); returns a key in eax
CSF_FUNC(int, __cdecl, TempStringHashValue, 0x0066B590, (void* tempObjValue));

static char __fastcall SpawnPoints_ActivateSpawn(C_00858180* self, void* /*edx*/, char a2, char a3, char a4, char a5)
{
    // Temporary string object on the stack (lives at [ebp-0x10], room up to [ebp-4])
    alignas(8) uint8_t tempStringBytes[12];
    CSF_AT(void*, tempStringBytes, 0) = self;   // object's first dword is the vtable slot = this

    // InitializeTemporaryString's source parameter is declared as a 32-bit integer,
    // so the string's address is passed as its integer value.
    const std::int32_t clavateSource =
        static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(CSF_STR(0x008679D8, "Clavate")));
    InitializeTemporaryString(tempStringBytes, nullptr, clavateSource);
    int lookupKey = TempStringHashValue(CSF_AT(void*, tempStringBytes, 0));

    C_008622e8* spawnTable = reinterpret_cast<C_008622e8*>(sub_64AD60(self, nullptr, (int)(&lookupKey)));

    GameConfig_WriteDwordField(spawnTable, nullptr, a2);
    GameConfig_WriteDwordField(spawnTable, nullptr, a3);
    GameConfig_WriteDwordField(spawnTable, nullptr, a4);
    GameConfig_WriteDwordField(spawnTable, nullptr, a5);

    return sub_64CEB0(self, nullptr, 0, spawnTable);
}

HOOK(0x00742360, SpawnPoints_ActivateSpawn, "auto");
