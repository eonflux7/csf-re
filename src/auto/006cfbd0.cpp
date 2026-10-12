#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Releases the particle emitters of the sub-object embedded at +4 (tail-jumps to the release routine).
CSF_FUNC(char, __fastcall, ParticleEmitters_ReleaseAll, 0x006E08D0, (void* self, void* edx));

static int __fastcall CollisionShapes_ReleaseEmittersAtOffset4(char* self, void* /*edx*/)
{
    return ParticleEmitters_ReleaseAll(self + 4, nullptr);
}

HOOK(0x006CFBD0, CollisionShapes_ReleaseEmittersAtOffset4, "auto");
