#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies the dword pointed to by a2 into this+0x30 and the dword pointed to
// by a3 into this+0x34, then returns a3. Callee pops 8 bytes (two stack args).
static uint32_t* __fastcall ParticleRender_CopyTwoDwordsAtOffsets30And34(
    void* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    CSF_AT(uint32_t, self, 0x30) = *a2;
    CSF_AT(uint32_t, self, 0x34) = *a3;
    return a3;
}

HOOK(0x705210, ParticleRender_CopyTwoDwordsAtOffsets30And34, "auto");
