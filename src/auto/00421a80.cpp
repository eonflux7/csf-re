#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Clears bit 3 (0x8) of the 32-bit flags word at +0x1F0 inside the object
// referenced by self->owner->+0x14, and returns a pointer to that flags word.
// Original: `mov eax,[ecx+8]; mov eax,[eax+0x14]; ... and ecx,0xfffffff7; mov [eax],ecx; ret`
static uint32_t* __fastcall TaskStates_ClearFlagBit3(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x08);
    void* inner = CSF_AT(void*, owner, 0x14);

    CSF_AT(uint32_t, inner, 0x1F0) &= 0xFFFFFFF7u;
    return &CSF_AT(uint32_t, inner, 0x1F0);
}

HOOK(0x00421A80, TaskStates_ClearFlagBit3, "auto");
