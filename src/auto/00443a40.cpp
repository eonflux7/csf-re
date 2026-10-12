#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Calls virtual slot 0 of `self` once per entry of the list, passing the entry value.
// `list` layout: [0] = pointer to uint32_t array, [2] = element count.
// Returns the result of the last slot call, or `list` itself when the count is zero.
static int __fastcall ForwardEachEntryToSlot0(void* self, void* /*edx*/, uint32_t* list) {
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(list));
    const uint32_t count = CSF_AT(uint32_t, list, 8);
    for (uint32_t index = 0; index < count; ++index) {
        const uint32_t entryValue = CSF_AT(uint32_t*, list, 0)[index];
        using Slot0Fn = int(__fastcall*)(void*, void*, uint32_t);
        Slot0Fn slot0 = reinterpret_cast<Slot0Fn>(CSF_AT(void**, self, 0)[0]);
        result = slot0(self, nullptr, entryValue);
    }
    return result;
}

HOOK(0x443A40, ForwardEachEntryToSlot0, "auto");
