#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TDistraido_Destroy(TDistraido* self, void* /*edx*/, char flags)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 16), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 12), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 8), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 4), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<char*>(self);
}

HOOK(0x004B2BA0, TDistraido_Destroy, "auto");
