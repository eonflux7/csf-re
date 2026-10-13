#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator (__cdecl, caller cleans its single argument).
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Allocates a 4-byte object whose first dword points at the audio effect
// vtable-like table at 0x863A58. Returns nullptr when allocation fails.
// Stdcall with two unused arguments; the callee pops 8 bytes.
static uint32_t* __stdcall AudioEffects_AllocateTableObject(int /*a1*/, int /*a2*/) {
    uint32_t* result = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(static_cast<uint32_t>(CoreMemory_AllocUntagged(4))));
    if (result == nullptr) {
        return nullptr;
    }
    *result = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00863A58)));
    return result;
}

HOOK(0x0070EC50, AudioEffects_AllocateTableObject, "auto");
