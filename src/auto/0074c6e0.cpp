#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (original addresses, signatures as given in the packet).
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(int, __cdecl, HashTemporaryString, 0x0066B590, (std::uint32_t tempObjectWord));

// Registers the "master has logged in" notification key with the stats screen's
// config table and returns AddConfigEntry's result.
static char __fastcall CMenuMpStats_RegisterMasterLoginNotification(CMenuMpStats* self, void* /*edx*/)
{
    // Temporary 8-byte object at [ebp-0xC]: first dword is `self`, the dword at
    // offset 4 ([ebp-4]) receives the hash of the first dword.
    alignas(8) std::uint8_t tempObjBytes[8];
    *reinterpret_cast<CMenuMpStats**>(tempObjBytes) = self;

    InitializeTemporaryString(tempObjBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x008682BC, "NotificaYaConectadosMasterHaEntradoEnSesion")));

    *reinterpret_cast<int*>(tempObjBytes + 4) =
        HashTemporaryString(*reinterpret_cast<std::uint32_t*>(tempObjBytes));

    void* configEntry = FindOrCreateConfigEntry(self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(tempObjBytes + 4)));

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074C6E0, CMenuMpStats_RegisterMasterLoginNotification, "auto");
