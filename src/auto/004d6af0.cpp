#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Float constant 0.0f at 0x82D4CC (g_fZero).
CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Returns true when the spawn point's float at +0x58 is nonzero (NaN counts as nonzero),
// otherwise returns the low bit of the byte before the object pointed to by this->owner->field_18.
static bool __fastcall MpSpawnPoints_IsSpecialObjectActive(void* self, void* /*edx*/)
{
    if (CSF_AT(float, self, 0x58) != g_fZero)
        return true;

    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x14));
    uint32_t linkedObject = CSF_AT(uint32_t, owner, 0x18);

    if (linkedObject == 0)
        return false;

    uint8_t flagsByte = *(reinterpret_cast<uint8_t*>(linkedObject) - 9);
    return (flagsByte & 1) != 0;
}

HOOK(0x004D6AF0, MpSpawnPoints_IsSpecialObjectActive, "auto");
