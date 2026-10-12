#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (original addresses, signatures as given in the packet).
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void*, __fastcall, FindOrCreateConfigEntry, 0x0064AD60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, AddConfigEntry, 0x0064CEB0, (void* self, void* edx, char a2, void* a3));
CSF_FUNC(std::uint32_t, __fastcall, HashTemporaryString, 0x0066B590, (void* self, void* edx));

// Resets the respawn-point memories: builds a temporary string object from the
// "ReseteaMemoriasPuntosRespawn" constant, hashes it, looks up or creates the
// config entry for that hash, and registers the entry with this object.
static char __fastcall C_00858180_ResetRespawnMemoriesConfig(C_00858180* self, void* /*edx*/)
{
    // Temporary 8-byte object at [ebp-0xC]: its first dword holds `self`
    // (the original stores this as the object's vtable/first word), and the
    // callee initializes it from the string constant.
    alignas(8) std::uint8_t tempObjBytes[8];
    *reinterpret_cast<C_00858180**>(tempObjBytes) = self;

    InitializeTemporaryString(tempObjBytes, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x008682E8, "ReseteaMemoriasPuntosRespawn")));

    // Hash result is stored at [ebp-4] and its address is handed to FindOrCreateConfigEntry.
    std::uint32_t hashedNameField = 0;
    hashedNameField = HashTemporaryString(tempObjBytes, nullptr);

    void* configEntry = FindOrCreateConfigEntry(self, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(&hashedNameField)));

    return AddConfigEntry(self, nullptr, 1, configEntry);
}

HOOK(0x0074C720, C_00858180_ResetRespawnMemoriesConfig, "auto");
