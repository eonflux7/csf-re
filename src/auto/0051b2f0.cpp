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

CSF_FUNC(int, __fastcall, S_0050f230_InsertUniqueKey, 0x00519EC0, (S_0050f230* self, void* /*edx*/, int resultAddress, uint32_t* value));

// Looks up the weapon def record for weaponIndex (records are 0x228 bytes each),
// inserts it into the sub-table at +0x2c, then ORs flagBits into the weapon flag byte
// at weaponIndex and returns the address of that byte.
static uint8_t* __fastcall C_00840680_OrWeaponFlag(C_00840680* self, void* /*edx*/, int weaponIndex, char flagBits)
{
    int resultKey;

    uint32_t recordsPtr = self->m_pRecords;
    uint32_t entryBase = CSF_AT(uint32_t, recordsPtr, 4);
    uint32_t* recordPtr = reinterpret_cast<uint32_t*>(entryBase + static_cast<uint32_t>(weaponIndex) * 0x228u);

    S_0050f230_InsertUniqueKey(
        reinterpret_cast<S_0050f230*>(reinterpret_cast<uint8_t*>(self) + 0x2c),
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(&resultKey)),
        recordPtr);

    uint8_t* flagBase = reinterpret_cast<uint8_t*>(self->m_pWeaponFlags);
    uint8_t* result = flagBase + weaponIndex;
    *result = static_cast<uint8_t>(flagBits | *result);
    return result;
}

HOOK(0x0051B2F0, C_00840680_OrWeaponFlag, "auto");
