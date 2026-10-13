#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies a sound-definition entry from a source record into this entry.
// The callee copies the common header first; then the dword fields at +0x10..+0x18
// and the byte-sized flags at +0x1C..+0x23 are copied one by one.
CSF_FUNC(void*, __fastcall, S_0046bea0_Copy, 0x00410E70, (void* self, void* edx, int a2));

static int __fastcall SoundDefs_CopyEntryFrom(S_0046bea0* self, void* /*edx*/, int a2)
{
    S_0046bea0_Copy(self, nullptr, a2);

    void* source = reinterpret_cast<void*>(a2);

    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, source, 0x10);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, source, 0x14);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, source, 0x18);

    CSF_AT(uint8_t, self, 0x1C) = CSF_AT(uint8_t, source, 0x1C);
    CSF_AT(uint8_t, self, 0x1D) = CSF_AT(uint8_t, source, 0x1D);
    CSF_AT(uint8_t, self, 0x1E) = CSF_AT(uint8_t, source, 0x1E);
    CSF_AT(uint8_t, self, 0x1F) = CSF_AT(uint8_t, source, 0x1F);
    CSF_AT(uint8_t, self, 0x20) = CSF_AT(uint8_t, source, 0x20);
    CSF_AT(uint8_t, self, 0x21) = CSF_AT(uint8_t, source, 0x21);
    CSF_AT(uint8_t, self, 0x22) = CSF_AT(uint8_t, source, 0x22);
    CSF_AT(uint8_t, self, 0x23) = CSF_AT(uint8_t, source, 0x23);

    return reinterpret_cast<int>(self);
}

HOOK(0x0075A6C0, SoundDefs_CopyEntryFrom, "auto");
