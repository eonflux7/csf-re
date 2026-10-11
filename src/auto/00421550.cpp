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

CSF_FUNC(void, __fastcall, sub_4DB3C0, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the three embedded sub-objects (at +0x28, +0x14 and +0x00) in that
// order, then frees the object itself through the game heap when bit 0 of
// a2 is set. Returns this.
static char* __fastcall TLanzarObjetoAPos_DestroySubobjectsAndMaybeFree(TLanzarObjetoAPos* self, void* /*edx*/, char a2)
{
    sub_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);
    sub_4DB3C0(reinterpret_cast<uint8_t*>(self) + 0x14, nullptr);
    sub_4DB3C0(reinterpret_cast<void*>(self), nullptr);

    if ((a2 & 1) != 0)
        Mem_Free(reinterpret_cast<void*>(self));

    return reinterpret_cast<char*>(self);
}

HOOK(0x421550, TLanzarObjetoAPos_DestroySubobjectsAndMaybeFree, "auto");
