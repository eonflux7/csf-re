#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpMTEffectGetInt, 0x007E13D0, (uint32_t* a1, int a2, int a3, uint32_t* a4));

// Refreshes the cached effect value (+0x48) of the audio effect entry held by this object.
// If the entry has an active RenderWare effect handle (+0x30) with flag bit 2 set in +0x34,
// the value is queried from the effect; otherwise the stored value at +0x48 is copied back.
static int __fastcall C_00863dd0_RefreshEffectValue(C_00863dd0* self, void* /*edx*/)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(self->field_0004);

    // Entry must be enabled (+0x50 non-zero), otherwise nothing is refreshed.
    if (!CSF_AT(uint32_t, entry, 0x50))
        return 0;

    uint32_t effectHandle = CSF_AT(uint32_t, entry, 0x30);
    if (effectHandle && (CSF_AT(uint8_t, entry, 0x34) & 2) != 0)
    {
        int effectValue = RpMTEffectGetInt(
            reinterpret_cast<uint32_t*>(effectHandle), 7, 2, nullptr);
        // Re-read the entry pointer: the callee may have changed self->field_0004.
        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x48) =
            static_cast<uint32_t>(effectValue);
        return effectValue;
    }

    int cachedValue = static_cast<int>(CSF_AT(uint32_t, entry, 0x48));
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0004), 0x48) =
        static_cast<uint32_t>(cachedValue);
    return cachedValue;
}

HOOK(0x712630, C_00863dd0_RefreshEffectValue, "auto");
