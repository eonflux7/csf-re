#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TEstatico_Destroy(TEstatico* self, void* /*edx*/, char flags)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x1C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x18), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x14), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x04), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);

    if ((flags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x004B2C90, TEstatico_Destroy, "auto");
