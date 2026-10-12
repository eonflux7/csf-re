#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, WriteConfigPair, 0x00656480, (void* self, void* edx, char a2, int a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

// Registers a "NotificaHayQueMigrar" config entry on this object, writes three
// key/value pairs into it, then adds it (flag 1) to the object's entry list.
static char __fastcall C_00858180_WriteMigrationNotifyEntry(C_00858180* self, void* /*edx*/,
                                                            char a2, int a3,
                                                            char a4, int a5,
                                                            char a6, int a7)
{
    std::uint32_t tempObjectWord = 0;
    InitializeTemporaryString(&tempObjectWord, nullptr,
                              reinterpret_cast<std::int32_t>(CSF_STR(0x0086847C, "NotificaHayQueMigrar")));
    int hashKey = HashTemporaryString(tempObjectWord);

    void* configEntry = FindOrCreateConfigEntry(self, nullptr, reinterpret_cast<int>(&hashKey));

    WriteConfigPair(configEntry, nullptr, a2, a3);
    WriteConfigPair(configEntry, nullptr, a4, a5);
    WriteConfigPair(configEntry, nullptr, a6, a7);

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074CCA0, C_00858180_WriteMigrationNotifyEntry, "auto");
