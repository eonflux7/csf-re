#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Copies a 0xE4-byte state block from the argument into the object at +0x18C
// and marks the flag at +0x188 as set. Returns the object.
static uint8_t* __fastcall BddLoader_ApplyScratchStateHelper(uint8_t* self, void* /*edx*/, const void* sourceState) {
    std::memcpy(self + 0x18C, sourceState, 0xE4);
    CSF_AT(uint8_t, self, 0x188) = 1;
    return self;
}

HOOK(0x00536A80, BddLoader_ApplyScratchStateHelper, "auto");
