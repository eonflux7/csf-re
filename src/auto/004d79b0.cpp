#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down the five embedded CharTaskBase sub-objects (at +0x50, +0x3C, +0x28, +0x14, +0x00),
// then frees the whole object with the game heap when bit 0 of the flag argument is set.
static char* __fastcall TAutonomo_DestroyAndMaybeFree(TAutonomo* self, void* /*edx*/, char flags)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    CharTaskBase_Init_4DB3C0(base + 0x50, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x3C, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(self, nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x4D79B0, TAutonomo_DestroyAndMaybeFree, "auto");
