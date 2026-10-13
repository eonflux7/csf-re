// Destroys a CLightObject: resets its vtables, tears down the embedded
// light state, runs the base-class destructor, and frees the block when bit 0 of a2 is set.
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

CSF_FUNC(void, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __fastcall, sub_6FCD40, 0x006FCD40, (void* self, void* edx));

static uint32_t* __fastcall CLightObject_DestructAndMaybeFree(C_00862ef8* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00862F70);
    CSF_AT(uint32_t, self, 0x4) = CSF_ADDR_DATA(0x00862F54);

    sub_6FCD40(self, nullptr);

    GfxObject_DestroyEmbeddedState((C_00860630*)((uint8_t*)self + 0x10), nullptr);

    CSF_AT(void*, self, 0x4) = CSF_VTBL(0x0082D410);
    *(void***)self = CSF_VTBL(0x00862EF8);

    C_00860170_Construct((C_00860170*)self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006FD4F0, CLightObject_DestructAndMaybeFree, "auto");
