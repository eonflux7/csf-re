#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls slot 7 (offset 0x1C) on each non-null child pointer stored at this+0x0C, +0x10, +0x14.
// Returns the value of the last slot visited (the last child's call result, or 0 if that child is null).
static int __fastcall Hud_ResetWidgetState(int* self, void* /*edx*/) {
    int result = 0;
    int* slot = self + 3;
    for (int remaining = 3; remaining != 0; --remaining) {
        result = *slot;
        if (result != 0) {
            void* element = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(result)));
            using SlotFn = int(__fastcall*)(void* self, void* edx);
            SlotFn fn = (SlotFn)CSF_AT(void**, element, 0)[7];
            result = fn(element, nullptr);
        }
        ++slot;
    }
    return result;
}

HOOK(0x00427590, Hud_ResetWidgetState, "auto");
