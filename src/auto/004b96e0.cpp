#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiCore_DestroyEmbeddedTask, 0x004AEA30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, TOrden_DestroyOrderTask, 0x004B2CF0, (TOrden* self, void*));
CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the embedded task at +0xCC, the order task at +0x08, and resets the
// vtables of the sub-objects at +0x04 and of self. Frees the block when bit 0
// of the argument is set. Returns `this`.
static int* __fastcall TFncMacroOrden_Destroy(TFncMacroOrden* self, void* /*edx*/, char freeMemory)
{
    AiCore_DestroyEmbeddedTask(reinterpret_cast<uint8_t*>(self) + 0xCC, nullptr);
    TOrden_DestroyOrderTask(reinterpret_cast<TOrden*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(self), nullptr);

    if ((freeMemory & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<int*>(self);
}

HOOK(0x004B96E0, TFncMacroOrden_Destroy, "auto");
