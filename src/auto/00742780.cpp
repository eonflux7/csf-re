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
CSF_FUNC(int, __cdecl, Sub_66B590, 0x0066B590, (std::uint32_t tempObjectWord));
CSF_FUNC(void*, __fastcall, sub_64AD60, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_64CEB0, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(void*, __fastcall, GameConfig_WriteWordField, 0x006563D0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

// Looks up the "RecogerArma" config entry, writes a word from a2 and a dword from a3 into it,
// then commits the entry through sub_64CEB0 with flag 1.
static char __fastcall C_00858180_WriteRecogerArmaEntry(C_00858180* self, void* /*edx*/, char a2, char a3)
{
    // Temporary string object: first dword holds the owner pointer.
    alignas(8) std::uint8_t tempStringBytes[0xC];
    CSF_AT(void*, tempStringBytes, 0) = self;

    InitializeTemporaryString(tempStringBytes, nullptr, reinterpret_cast<std::int32_t>(CSF_STR(0x008679F0, "RecogerArma")));
    int entryId = Sub_66B590(CSF_AT(std::uint32_t, tempStringBytes, 0));

    // The original passes the address of entryId (&v7), not its value.
    void* configEntry = sub_64AD60(self, nullptr, static_cast<int>(reinterpret_cast<std::uintptr_t>(&entryId)));
    GameConfig_WriteWordField(configEntry, nullptr, a2);
    GameConfig_WriteDwordField(configEntry, nullptr, a3);
    return sub_64CEB0(self, nullptr, 1, configEntry);
}

HOOK(0x00742780, C_00858180_WriteRecogerArmaEntry, "auto");
