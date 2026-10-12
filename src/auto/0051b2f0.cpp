#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Loads the 552-byte (0x228) weapon-def record for index `a2` into a scratch buffer.
CSF_FUNC(int, __fastcall, WeaponDefs_ReadRecord, 0x00519EC0, (void* self, void* edx, int a2, uint32_t* a3));

// OR-s `flagBits` into the weapon flag byte at index `index`, after reading that
// index's record into a temporary buffer. Returns a pointer to the updated flag byte.
static uint8_t* __fastcall C_00840680_OrWeaponFlag(C_00840680* self, void* /*edx*/, int index, char flagBits)
{
    // Scratch output for the record reader (original reserves 8 bytes at [ebp-8]).
    uint32_t recordScratch[2];

    // Record base: dword at m_pRecords+4, then 552 bytes per index.
    uint8_t* recordBase = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(self->m_pRecords + 4));
    uint32_t* recordPtr = reinterpret_cast<uint32_t*>(recordBase + static_cast<ptrdiff_t>(index) * 552);

    WeaponDefs_ReadRecord(reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr, reinterpret_cast<int>(recordPtr), recordScratch);

    // Flags are re-read after the call (the callee may have changed the object).
    uint8_t* flagBase = reinterpret_cast<uint8_t*>(self->m_pWeaponFlags);
    uint8_t currentFlags = flagBase[index];
    uint8_t* flagSlot = flagBase + index;
    *flagSlot = static_cast<uint8_t>(static_cast<uint8_t>(flagBits) | currentFlags);
    return flagSlot;
}

HOOK(0x0051B2F0, C_00840680_OrWeaponFlag, "auto");
