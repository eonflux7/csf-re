#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x691110: game heap deallocation (the exact allocator this destructor's caller expects).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// 0x4DBDB0: base-agent teardown that runs after the vtable pointers are restored and
// before the optional free.
CSF_FUNC(int, __fastcall, AiCore_DestroyBaseAgent, 0x004DBDB0, (void* self, void* edx));

// C_00835398 destructor: restores the primary vtable (offset 0x00) and the secondary
// vtable (offset 0x04), runs the base-agent teardown and, when the low bit of the flag
// argument is set, frees the object on the game heap. Returns the object pointer.
static C_00835398* __fastcall C_00835398_Destroy(C_00835398* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00835398);
    self->field_0004 = CSF_ADDR_VT(0x00835310);

    AiCore_DestroyBaseAgent(self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return self;
}

HOOK(0x004A9C50, C_00835398_Destroy, "auto");
