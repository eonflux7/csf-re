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

// Releases the embedded sub-object at +0x04, resets the vtable, and frees the
// block when bit 0 of the argument is set. Returns the object pointer.

CSF_FUNC(int, __fastcall, ReleaseEmbeddedMember, 0x0044AFF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_0082d410* __fastcall C_0082d410_DestroyAndMaybeFree(C_0082d410* self, void* /*edx*/, char a2)
{
    ReleaseEmbeddedMember(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    *(void***)self = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x465260, C_0082d410_DestroyAndMaybeFree, "auto");
