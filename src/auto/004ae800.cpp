#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Tears down the five embedded C_0082d410 sub-objects (at +0x10, +0x0C, +0x08, +0x04, +0x00),
// then frees the whole object with the game heap when bit 0 of the flag argument is set.
static char* __fastcall TMovilAPunto_2_DestroyAndMaybeFree(TMovilAPunto_2* self, void* /*edx*/, char flags)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(base + 0x10), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(base + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(base + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(base + 0x04), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);

    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x4AE800, TMovilAPunto_2_DestroyAndMaybeFree, "auto");
