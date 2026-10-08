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

// Callees and data bound by address.
CSF_FUNC(void*, __fastcall, SaveGameSubObject_Init, 0x697130, (void* self, void* /*edx*/, uint8_t* flagByte));
CSF_FUNC(int, __cdecl, MenuSaveGame_ClearSlotRange, 0x544340, (int* firstSlot, int* lastSlot, void* menuSlots, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x690E60, (void* block));
CSF_GLOBAL(uint8_t, g_SaveGameFlagByte, 0x82E65C);

// Resets a CMenuSaveGame instance: clears the slot-range pointers and counters,
// re-initializes the embedded sub-object at +0x14, clears the flag byte at +0x30,
// releases the slot buffer if one exists, and zeroes the slot-range fields again.
// Returns the object pointer (this).
static void* __fastcall MenuSaveGame_ResetState(void* self, void* /*edx*/)
{
    uint8_t* base = static_cast<uint8_t*>(self);

    // Slot-range block at +4: buffer pointer at +8, end pointer at +0xC, count at +0x10.
    CSF_AT(uint32_t, base, 0x08) = 0;
    CSF_AT(uint32_t, base, 0x0C) = 0;
    CSF_AT(uint32_t, base, 0x10) = 0;

    // Embedded sub-object at +0x14.
    CSF_AT(uint32_t, base, 0x2C) = 7;
    CSF_AT(uint32_t, base, 0x28) = 0;
    CSF_AT(uint16_t, base, 0x18) = 0;

    CSF_AT(uint32_t, base, 0x00) = 0;

    SaveGameSubObject_Init(base + 0x14, nullptr, &g_SaveGameFlagByte);

    CSF_AT(uint8_t, base, 0x30) = 0;

    // Release the slot buffer if one is present. The original re-reads the
    // buffer pointer at each step, so do the same.
    if (CSF_AT(uint32_t, base, 0x08) != 0) {
        MenuSaveGame_ClearSlotRange(
            reinterpret_cast<int*>(CSF_AT(uint32_t, base, 0x08)),
            reinterpret_cast<int*>(CSF_AT(uint32_t, base, 0x0C)),
            base + 4,
            self);
        if (CSF_AT(uint32_t, base, 0x08) != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, base, 0x08)));
        }
    }

    CSF_AT(uint32_t, base, 0x08) = 0;
    CSF_AT(uint32_t, base, 0x0C) = 0;
    CSF_AT(uint32_t, base, 0x10) = 0;

    return self;
}

HOOK(0x5445F0, MenuSaveGame_ResetState, "auto");
