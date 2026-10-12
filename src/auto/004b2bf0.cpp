#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TDudoso_Destroy(TDudoso* self, void* /*edx*/, char flags)
{
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(bytes + 0x20), nullptr);
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

HOOK(0x004B2BF0, TDudoso_Destroy, "auto");
