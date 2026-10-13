#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a 3-dword command handle into the AI mount-task object.
// src: pointer to a 3-dword record; words [1] and [2] go to this+0x1B8 and this+0x1BC.
// thirdValue: stored unchanged at this+0x1C0.
// Returns src[2] (the value written to this+0x1BC).
static int __fastcall AiMountTasks_CopyCommandHandle(void* self, void* /*edx*/, void* src, uint32_t thirdValue)
{
    CSF_AT(uint32_t, self, 0x1B8) = CSF_AT(uint32_t, src, 4);
    uint32_t result = CSF_AT(uint32_t, src, 8);
    CSF_AT(uint32_t, self, 0x1BC) = result;
    CSF_AT(uint32_t, self, 0x1C0) = thirdValue;
    return static_cast<int>(result);
}

HOOK(0x00492AE0, AiMountTasks_CopyCommandHandle, "auto");
