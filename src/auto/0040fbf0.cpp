#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: SceneLoader_NoOp (__cdecl, no args, no-op).
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Original globals: the zero vector components.
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Resets the object's vector position to zero and restores its default state flags.
static int __fastcall C_0082deb8_ResetToZeroVectorAndDefaults(C_0082deb8* self, void* /*edx*/) {
    SceneLoader_NoOp();

    self->field_0018 = g_ZeroVectorX;
    CSF_AT(float, self, 0x1C) = g_ZeroVectorY;
    CSF_AT(float, self, 0x20) = g_ZeroVectorZ;

    self->field_003d = 1;
    self->field_003e = 0;
    self->field_0078 = 0;
    self->field_007c = 0xFFFFFFFFu;

    return 0;
}

HOOK(0x0040FBF0, C_0082deb8_ResetToZeroVectorAndDefaults, "auto");
