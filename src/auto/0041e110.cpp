#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the child block at +0x08 (if any), clears the pointer, and zeroes +0x04.
static int __fastcall C_0082deb8_ReleaseChildBase(C_0082e520* self, void* /*edx*/)
{
    int result = 0;
    void* child = reinterpret_cast<void*>(self->field_0008);
    if (child) {
        SceneLoader_NoOp();
        result = Mem_Free(child);
        self->field_0008 = 0;
    }
    self->field_0004 = 0;
    return result;
}

HOOK(0x0041E110, C_0082deb8_ReleaseChildBase, "auto");
