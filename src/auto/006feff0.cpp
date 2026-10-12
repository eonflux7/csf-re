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

CSF_FUNC(void, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void* edx));
CSF_FUNC(char, __fastcall, ClumpLoader_ReleaseStreamState, 0x006FE470, (void* self, void* edx));

// Releases one owned heap block (game allocator) if present and clears its slot.
static void ReleaseOwnedBlock(uint32_t* slot)
{
    if (*slot)
    {
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(*slot)));
        *slot = 0;
    }
}

// Tears down the clump-loader object: releases stream state and owned buffers,
// then reverts to the C_00860170 base and constructs it.
static void __fastcall C_00863018_Destroy(C_00863018* self, void* /*edx*/)
{
    uint8_t wasStreamActive = self->field_0021;
    *(void***)self = CSF_VTBL(0x00863044);
    if (wasStreamActive)
        ClumpLoader_ReleaseStreamState(self, nullptr);

    if (self->field_0020)
    {
        if (self->field_0021)
            ClumpLoader_ReleaseStreamState(self, nullptr);
        self->field_0020 = 0;
    }

    ReleaseOwnedBlock(&self->field_0054);
    ReleaseOwnedBlock(&self->field_0044);
    ReleaseOwnedBlock(&self->field_0034);
    ReleaseOwnedBlock(&self->field_0024);

    *(void***)self = CSF_VTBL(0x00863018);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006FEFF0, C_00863018_Destroy, "auto");
