#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Zeroes the first 0x1C bytes of an emitter sub-object as two 4-dword blocks
// (values 0,0,0,10 and 0,0,0,10) and returns the object pointer.
static void* __fastcall ParticleAtomic_ResetEmitterSubObject(void* self, void* /*edx*/) {
    CSF_AT(int, self, 0x00) = 0;
    CSF_AT(int, self, 0x04) = 0;
    CSF_AT(int, self, 0x08) = 0;
    CSF_AT(int, self, 0x0C) = 10;
    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 10;
    return self;
}

HOOK(0x00703440, ParticleAtomic_ResetEmitterSubObject, "auto");
