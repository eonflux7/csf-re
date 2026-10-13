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

// Resets the string storage of the embedded sub-object at +0x08, restores the
// base vtable, and frees the block when bit 0 of the argument is set.
// Returns the object pointer (the original returns &this->__vftable == this).

CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static C_0082d410* __fastcall C_0082d410_DestroyAndMaybeFree(C_0082d410* self, void* /*edx*/, char a2)
{
    CMenuSaveGame_ResetStringStorage(reinterpret_cast<uint8_t*>(self) + 8, nullptr);

    *(void***)self = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x457060, C_0082d410_DestroyAndMaybeFree, "auto");
