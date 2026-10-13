#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxParticleAtomic_AddEmitter, 0x007034C0, (int* self, void* , int arrayIndex, int atomic));

// Forwards to GfxParticleAtomic_AddEmitter with `this` moved to the sub-object at +0x20.
// Tail call: the callee's `ret 8` pops the two stack arguments.
static int __fastcall GfxParticle_ForwardAddEmitterAt20(void* self, void* /*edx*/, int a1, int a2)
{
    int* subobject = reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x20);
    return GfxParticleAtomic_AddEmitter(subobject, nullptr, a1, a2);
}

HOOK(0x6E04C0, GfxParticle_ForwardAddEmitterAt20, "auto");
