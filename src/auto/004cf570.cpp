#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TCharTask_PrepareBase, 0x004DB3F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_GetDrawState, 0x0049CF10, (void* self, void* edx));

// Prepares the TCharTask base, clears the per-task counter at +0x40, and caches
// the draw-state value (dword at offset 0x1A0 of the scene object's draw state)
// into +0x44. Returns that cached value.
static int __fastcall C_0083b2e0_InitDrawStateSnapshot(C_0083b2e0* self, void* /*edx*/)
{
    TCharTask_PrepareBase(self, nullptr);

    void* sceneObject = reinterpret_cast<void*>(self->field_0014);
    self->field_0040 = 0;

    void* drawState = reinterpret_cast<void*>(
        static_cast<uintptr_t>(SceneObject_GetDrawState(sceneObject, nullptr)));
    int result = CSF_AT(int, drawState, 0x1A0);

    self->field_0044 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x004CF570, C_0083b2e0_InitDrawStateSnapshot, "auto");
