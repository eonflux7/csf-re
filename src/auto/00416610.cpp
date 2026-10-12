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

CSF_FUNC(int, __fastcall, C_0082deb8_ReleaseChildBase_41E110, 0x0041E110, (C_0082e520* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor body for C_0082e090: resets the vtables to the base chain,
// releases the base-class child state, and frees the block when a2 bit 0 is set.
static uint32_t* __fastcall C_0082e090_DestroyAndMaybeFree(C_0082e090* self, void* /*edx*/, char a2)
{
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0082E090);

    C_0082deb8_ReleaseChildBase_41E110(self, nullptr);

    CSF_AT(void*, self, 0xAC) = CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00416610, C_0082e090_DestroyAndMaybeFree, "auto");
