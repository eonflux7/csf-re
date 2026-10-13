#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));

// Clears the marker words of the 28-byte entry selected by a2 and drops the
// "marked" bit (0x8) from its flags. If the entry's bit 0x4 is set, the owner
// gets bit 0x200 at +0x170.
static int __fastcall C_Unknown_ResetEntryAndFlagOwner(void* self, void* /*edx*/, C_00833908* a2)
{
    int entryOffset = 28 * C_Unknown_GetField_01c0(a2, nullptr);
    uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x198));
    uint8_t* entry = entries + entryOffset;

    CSF_AT(int32_t, entry, 0x4) = -1;
    CSF_AT(int32_t, entry, 0x8) = -1;

    CSF_AT(uint32_t, entry, 0x18) &= ~8u;

    if ((CSF_AT(uint8_t, entry, 0x18) & 4) != 0)
        CSF_AT(uint32_t, self, 0x170) |= 0x200u;

    return entryOffset;
}

HOOK(0x0049F140, C_Unknown_ResetEntryAndFlagOwner, "auto");
