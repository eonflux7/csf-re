#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));

// Appends `value` to the dynamic dword array at +0xE0 when bit 0x02 of the byte at +0x08 is set.
// Returns true when the value was appended, false when the flag is clear.
static bool __fastcall SkinAppendEntryIfEnabled(void* self, void* /*edx*/, int value) {
    [[maybe_unused]] const void* const growArrayVtbl = CSF_VTBL(0x0041E5F0);

    if ((CSF_AT(uint8_t, self, 0x08) & 0x02) == 0) {
        return false;
    }

    // Array header at +0xE0: data pointer (+0xE0), capacity (+0xE4),
    // count (+0xE8), growth step (+0xEC).
    if (CSF_AT(uint32_t, self, 0xE8) >= CSF_AT(uint32_t, self, 0xE4)) {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(static_cast<uint8_t*>(self) + 0xE0),
            nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0xEC)));
    }

    // Reload count and data pointer after a possible grow.
    uint32_t index = CSF_AT(uint32_t, self, 0xE8);
    uint32_t* slots = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, self, 0xE0));
    slots[index] = static_cast<uint32_t>(value);
    CSF_AT(uint32_t, self, 0xE8) = index + 1;
    return true;
}

HOOK(0x006C62F0, SkinAppendEntryIfEnabled, "auto");
