#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Ors a flag byte into the per-weapon flag array at m_pWeaponFlags[index].
// Returns a pointer to the byte that was updated.
static uint8_t* __fastcall CWeaponDatabase_OrWeaponFlag(CWeaponDatabase* self, void* /*edx*/, int weaponIndex, char flagBits)
{
    uint8_t* flags = reinterpret_cast<uint8_t*>(self->m_pWeaponFlags);
    uint8_t* flagByte = flags + weaponIndex;
    *flagByte = static_cast<uint8_t>(*flagByte | static_cast<uint8_t>(flagBits));
    return flagByte;
}

HOOK(0x0051AF10, CWeaponDatabase_OrWeaponFlag, "auto");
