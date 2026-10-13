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

// Registers the "Die" config entry for this stats menu: builds a hashed key from the
// temporary string "Die" and passes it to the config table, then adds the entry.
static char __fastcall CMenuMpStats_RegisterDieConfigEntry(CMenuMpStats* self, void* /*edx*/)
{
    // Temporary object at [ebp-0xC]: first dword is `self` (the original pushes ecx there),
    // the hashed key lives at offset 8 ([ebp-4]).
    alignas(8) std::uint8_t tempObjBytes[12];
    *reinterpret_cast<CMenuMpStats**>(tempObjBytes) = self;

    InitializeTemporaryString(tempObjBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x008687D4, "Die")));

    // HashTemporaryString takes the object's first dword (as it stands after the
    // initialization call) as its single cdecl argument; the result goes to [ebp-4].
    std::uint32_t tempObjectWord = *reinterpret_cast<std::uint32_t*>(tempObjBytes);
    *reinterpret_cast<std::uint32_t*>(tempObjBytes + 8) =
        static_cast<std::uint32_t>(HashTemporaryString(tempObjectWord));

    void* configEntry = FindOrCreateConfigEntry(self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(tempObjBytes + 8)));

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074F890, CMenuMpStats_RegisterDieConfigEntry, "auto");
