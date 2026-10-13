#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the address of sound descriptor `soundIndex` (68-byte records).
// self+0x004 holds a pointer to a table header; the descriptors base is the dword at header+4.
static int __fastcall AudioSoundManager_GetSoundDescriptor(void* self, void* /*edx*/, int soundIndex) {
    uint32_t headerAddress = CSF_AT(uint32_t, self, 0x004);
    uint8_t* header = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(headerAddress));
    uint32_t descriptorsBase = CSF_AT(uint32_t, header, 4);
    uint32_t descriptorOffset = static_cast<uint32_t>(soundIndex) * 0x44u;
    return static_cast<int>(descriptorsBase + descriptorOffset);
}

HOOK(0x0075A480, AudioSoundManager_GetSoundDescriptor, "auto");
