#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));

static void __fastcall TFncRastrear_DestroyTrackingTask(TFncRastrear* self, void* /*edx*/)
{
    if (CSF_AT(void*, self, 0x20))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x20));
        CSF_AT(void*, self, 0x20) = nullptr;
    }
    if (CSF_AT(void*, self, 0x10))
    {
        CoreMemory_Free(CSF_AT(void*, self, 0x10));
        CSF_AT(void*, self, 0x10) = nullptr;
    }

    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);
}

HOOK(0x004AE700, TFncRastrear_DestroyTrackingTask, "auto");
