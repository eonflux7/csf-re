#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Clears the two string-storage members, restores the vtable, and frees the object when requested.
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_0082d410_Destroy(C_0082d410* self, void* /*edx*/, char deleteAfter)
{
    CMenuSaveGame_ResetStringStorage(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr);
    CMenuSaveGame_ResetStringStorage(&self->field_0008, nullptr);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    if ((deleteAfter & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00457020, C_0082d410_Destroy, "auto");
