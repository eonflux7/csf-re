#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, WeaponDefs_RebuildEntryTable, 0x00783670, (int a1, int a2, int a3, int a4));
CSF_FUNC(char*, __cdecl, WeaponDefs_LookupEntryName, 0x00784210, (int a1, int a2, char* a3));

static char** __fastcall WeaponDefs_UpdateCurrentEntry(
    void* self,
    void* /*edx*/,
    char** outputSlot,
    char* currentEntry,
    char* previousEntry)
{
    if (currentEntry != previousEntry)
    {
        char* newEntryTable = WeaponDefs_LookupEntryName(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(previousEntry)),
            static_cast<int>(CSF_AT(uint32_t, self, 0x08)),
            currentEntry);

        WeaponDefs_RebuildEntryTable(
            static_cast<int>(reinterpret_cast<std::uintptr_t>(newEntryTable)),
            static_cast<int>(CSF_AT(uint32_t, self, 0x08)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
            static_cast<int>(reinterpret_cast<std::uintptr_t>(outputSlot)));

        CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(newEntryTable));
    }

    *outputSlot = currentEntry;
    return outputSlot;
}

HOOK(0x00784440, WeaponDefs_UpdateCurrentEntry, "auto");
