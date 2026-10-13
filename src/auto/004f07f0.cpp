#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of animation definition `index` in a 40-byte-stride table
// whose base pointer is stored at +0x04 of this object.
static int __fastcall SceneMissionSetup_GetAnimationDefinition(void* self, void* /*edx*/, int index) {
    uint32_t tableBase = CSF_AT(uint32_t, self, 0x04);
    return static_cast<int>(tableBase + static_cast<uint32_t>(index) * 40u);
}

HOOK(0x004F07F0, SceneMissionSetup_GetAnimationDefinition, "auto");
