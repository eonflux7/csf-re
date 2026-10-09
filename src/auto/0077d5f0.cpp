#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap wrappers used by the original.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Per-entry destructor for the pointer table at field_0004.
CSF_FUNC(char, __fastcall, LevelEntry_Destruct, 0x00775090, (void* self, void* edx));
// Destructor for the index table at field_0018: (table, aux at field_001c, header at +0x14, count).
CSF_FUNC(int, __cdecl, LevelIndexTable_Destroy, 0x00775200, (int* a1, int* a2, int a3, int a4));
// Destructor for the block whose header is at +0x44: (data at +0x48, aux at +0x4C, header, count).
CSF_FUNC(int, __cdecl, LevelBlock44_Destroy, 0x007752E0, (int a1, int a2, int a3, int a4));
// Destructor for the block whose header is at +0x34: (data at +0x38, aux at +0x3C, header, count).
CSF_FUNC(int, __cdecl, LevelBlock34_Destroy, 0x007754B0, (int a1, int a2, int a3, int a4));
// Per-sector destructor, invoked on each 0x48-byte entry of the sector table at field_0028.
CSF_FUNC(char, __fastcall, LevelSectorEntry_Destruct, 0x00778140, (void* self, void* edx));

static int __fastcall CLevelData_ReleaseLoadedData(CLevelData* self, void* /*edx*/)
{
    // Scratch count. Starts as the entry count (field_000c); the original reuses the same
    // stack slot for the sector-table end pointer (field_002c) once that table is processed,
    // and the later block calls receive whichever value is currently in that slot.
    uint32_t sharedCount = self->field_000c;

    // 1) Entry table at field_0004: destroy and free each non-null entry.
    for (uint32_t index = 0; index < sharedCount; ++index) {
        void* entry = reinterpret_cast<void**>(self->field_0004)[index];
        if (entry != nullptr) {
            LevelEntry_Destruct(entry, nullptr);
            Mem_Free(entry);
            reinterpret_cast<void**>(self->field_0004)[index] = nullptr;
        }
    }

    // 2) Clear the entry counters and free the entry table itself.
    const uint32_t entryTable = self->field_0004;
    self->field_0008 = 0;
    self->field_000c = 0;
    if (entryTable != 0) {
        Mem_Free(reinterpret_cast<void*>(entryTable));
        self->field_0004 = 0;
    }

    // 3) Index table at field_0018, aux pointer at field_001c, header at +0x14.
    const uint32_t indexTable = self->field_0018;
    if (indexTable != 0) {
        LevelIndexTable_Destroy(reinterpret_cast<int*>(indexTable),
                                reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x1C)),
                                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14),
                                static_cast<int>(sharedCount));
        if (self->field_0018 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
        }
    }
    self->field_0018 = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;

    // 4) Sector table at field_0028 (0x48-byte stride), end pointer in field_002c.
    const uint32_t sectorTable = self->field_0028;
    if (sectorTable != 0) {
        sharedCount = self->field_002c;
        for (uint32_t sector = sectorTable; sector != sharedCount; sector += 0x48) {
            LevelSectorEntry_Destruct(reinterpret_cast<void*>(sector), nullptr);
        }
        if (self->field_0028 != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0028));
        }
    }
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;

    // 5) Block with header at +0x34: data at field_0038, aux at field_003c.
    uint8_t* header34 = reinterpret_cast<uint8_t*>(self) + 0x34;
    const uint32_t block34Data = CSF_AT(uint32_t, self, 0x38);
    if (block34Data != 0) {
        LevelBlock34_Destroy(static_cast<int>(block34Data),
                             static_cast<int>(CSF_AT(uint32_t, self, 0x3C)),
                             reinterpret_cast<int>(header34),
                             static_cast<int>(sharedCount));
        if (CSF_AT(uint32_t, self, 0x38) != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x38)));
        }
    }
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;

    // 6) Block with header at +0x44: data at field_0048, aux at field_004C.
    uint8_t* header44 = reinterpret_cast<uint8_t*>(self) + 0x44;
    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x48));
    if (result != 0) {
        LevelBlock44_Destroy(result,
                             static_cast<int>(CSF_AT(uint32_t, self, 0x4C)),
                             reinterpret_cast<int>(header44),
                             static_cast<int>(sharedCount));
        result = static_cast<int>(CSF_AT(uint32_t, self, 0x48));
        if (result != 0) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x48)));
        }
    }
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    return result;
}

HOOK(0x0077D5F0, CLevelData_ReleaseLoadedData, "auto");
