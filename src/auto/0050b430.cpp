#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Concurrency::details::_UnrealizedChore::_SetDetached
// __thiscall, one stack argument (bool), callee pops 4 bytes (ret 4).
// Stores the flag byte at this+0x11.
static void __fastcall UnrealizedChore_SetDetached(void* self, void* /*edx*/, bool detached) {
    CSF_AT(uint8_t, self, 0x11) = static_cast<uint8_t>(detached);
}

HOOK(0x0050B430, UnrealizedChore_SetDetached, "auto");
