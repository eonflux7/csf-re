#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the address of the spawn entry `index` steps back from the current cursor
// (field at +0xC84), wrapping by 50 (0x32) when the step goes negative. Entries are 64 bytes.
static int __fastcall SpawnPoints_EntryAddressForIndex(void* self, void* /*edx*/, int index) {
    int step = CSF_AT(int, self, 0xC84) - index - 1;
    if (step < 0) {
        step += 0x32;
    }
    uintptr_t base = reinterpret_cast<uintptr_t>(self);
    return static_cast<int>(base + 4 + (static_cast<uint32_t>(step) << 6));
}

HOOK(0x5512B0, SpawnPoints_EntryAddressForIndex, "auto");
