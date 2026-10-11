#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MenuSaveGame_ReleaseSlotBuffer, 0x005129A0,
         (uint32_t slotBuffer, uint32_t slotCount, void* slotOwnerSubobject, void* owner));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CSaveGameManager_ReleaseSlotsAndReset(CSaveGameManager* self, void* /*edx*/)
{
    // Sub-object embedded at +0x08 (lea esi, [ecx + 8]); passed as the slot owner.
    void* slotOwnerSubobject = reinterpret_cast<uint8_t*>(self) + 8;

    self->field_0018 = 1;

    int result = static_cast<int>(self->field_000c);
    if (result)
    {
        MenuSaveGame_ReleaseSlotBuffer(self->field_000c,
                                       CSF_AT(uint32_t, self, 0x10),
                                       slotOwnerSubobject,
                                       self);

        // Re-read the buffer pointer after the callee ran.
        result = static_cast<int>(self->field_000c);
        if (result)
        {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)));
        }
    }

    self->field_000c = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    return result;
}

HOOK(0x00514470, CSaveGameManager_ReleaseSlotsAndReset, "auto");
