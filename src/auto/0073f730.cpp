#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Attaches a two-way reference between `self` and the object passed on the stack.
// Original: __thiscall with one stack argument, `ret 4`. Returns the argument unchanged.
static int __fastcall SpawnPoints_AttachObjectReference(void* self, void* /*edx*/, int a2) {
    int result = a2;
    if (a2) {
        auto* attached = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
        auto* owner = static_cast<uint8_t*>(self);
        CSF_AT(uint32_t, attached, 0x08) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        CSF_AT(uint32_t, owner, 0x04) = static_cast<uint32_t>(a2);
    }
    return result;
}

HOOK(0x0073F730, SpawnPoints_AttachObjectReference, "auto");
