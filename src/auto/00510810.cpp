#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void* edx));

// Frees the heap buffer at +0x8C when its capacity (+0xA0) is at least 16, resets the
// buffer state (capacity 15, length 0, first byte 0), then resets the two embedded
// string storages at +0x24 and +0x08. Tail-calls the storage reset at +0x08.
static int __fastcall BddLoader_ResetStringStorage(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0xA0) >= 0x10u && CSF_AT(uint32_t, self, 0x8C) != 0) {
        void* buffer = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C));
        Mem_FreeDebug(buffer);
    }
    CSF_AT(uint32_t, self, 0xA0) = 15;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint8_t, self, 0x8C) = 0;

    CMenuSaveGame_ResetStringStorage(reinterpret_cast<C_0082d410*>(static_cast<uint8_t*>(self) + 0x24), nullptr);
    return CMenuSaveGame_ResetStringStorage(reinterpret_cast<C_0082d410*>(static_cast<uint8_t*>(self) + 0x08), nullptr);
}

HOOK(0x00510810, BddLoader_ResetStringStorage, "auto");
