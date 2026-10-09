#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the transient per-object state of an object whose parent record
// (pointer stored at +16) supplies a flag byte at offset +1 that is copied
// into the byte at +128. Returns 0.
static int __fastcall InitializeTransientState(void* self, void* /*edx*/)
{
    uint8_t* parentRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 16));
    CSF_AT(uint8_t, self, 128) = parentRecord[1];

    CSF_AT(uint32_t, self, 104) = 0;
    CSF_AT(uint32_t, self, 88) = 0;
    CSF_AT(uint32_t, self, 24) = 0;
    CSF_AT(uint32_t, self, 28) = 0;
    CSF_AT(uint32_t, self, 20) = 0;
    CSF_AT(uint8_t, self, 129) = 0;
    CSF_AT(uint8_t, self, 131) = 0;
    CSF_AT(int32_t, self, 96) = -1;
    CSF_AT(int32_t, self, 100) = -1;
    CSF_AT(uint8_t, self, 130) = 0;
    CSF_AT(int32_t, self, 108) = -1;
    CSF_AT(uint32_t, self, 112) = 0;
    CSF_AT(int32_t, self, 116) = -1;
    CSF_AT(uint32_t, self, 120) = 0;
    CSF_AT(uint32_t, self, 124) = 0;

    return 0;
}

// LTCG callers (0x462370, 0x4624E0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x4619C0, InitializeTransientState, 0, "auto");
