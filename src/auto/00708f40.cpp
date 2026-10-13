#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stores a3 at offset 0x28 of the entry at index a2 (44-byte stride) in the
// array pointed to by this->field_0054. Returns the entry's address.
static int __fastcall C_008629a4_ApplyBaseEffectParameter(C_008629a4* self, void* /*edx*/, int a2, int a3)
{
    const uint32_t entryBase = CSF_AT(uint32_t, self, 0x54);
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryBase + static_cast<uint32_t>(a2) * 44u));
    CSF_AT(int, entry, 0x28) = a3;
    return static_cast<int>(reinterpret_cast<uintptr_t>(entry));
}

HOOK(0x708F40, C_008629a4_ApplyBaseEffectParameter, "auto");
