#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, TCharTask_PrepareBase, 0x004DB3F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_GetDrawState, 0x0049CF10, (void* self, void* edx));

// Caches the draw-state parameter at +0x1A0 of the scene object's draw state
// into the float slot at +0x44 (bit pattern copied, as the original does),
// clears the counter at +0x40 and the float at +0x48, and returns the parameter.
static int __fastcall C_0083ad48_CacheDrawStateParam(C_0083ad48* self, void* /*edx*/)
{
    TCharTask_PrepareBase(self, nullptr);

    void* sceneObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0014));
    self->field_0040 = 0;

    int drawState = SceneObject_GetDrawState(sceneObject, nullptr);
    int drawStateParam = CSF_AT(int, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(drawState)), 0x1A0);

    reinterpret_cast<int&>(self->field_0044) = drawStateParam;
    self->field_0048 = 0.0f;
    return drawStateParam;
}

HOOK(0x004CE930, C_0083ad48_CacheDrawStateParam, "auto");
