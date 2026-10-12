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
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));

// Writes the "NotificaCogemosUnaClave" config entry: two byte fields from a2 and a3.
static char __fastcall C_00858180_WriteNotificaCogemosFields(C_00858180* self, void* /*edx*/, char a2, char a3)
{
    // Temporary string object: first dword holds the owner pointer.
    alignas(8) std::uint8_t tempStringBytes[0xC];
    CSF_AT(void*, tempStringBytes, 0) = self;

    InitializeTemporaryString(tempStringBytes, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x00868464, "NotificaCogemosUnaClave")));
    int hashValue = HashTemporaryString(CSF_AT(std::uint32_t, tempStringBytes, 0));

    // The original passes the address of the hash slot, not its value.
    void* configEntry = FindOrCreateConfigEntry(self, nullptr, reinterpret_cast<int>(&hashValue));
    GameConfig_WriteByteField(configEntry, nullptr, a2);
    GameConfig_WriteByteField(configEntry, nullptr, a3);
    return AddConfigEntry(self, nullptr, 0, configEntry);
}

HOOK(0x0074CC00, C_00858180_WriteNotificaCogemosFields, "auto");
