#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GameBddLoader_DestroyCharacterRecord(void* self, void* /*edx*/)
{
    auto* record = static_cast<uint8_t*>(self);

    CMenuSaveGame_ResetStringStorage(record + 0x1F4, nullptr);
    CMenuSaveGame_ResetStringStorage(record + 0x198, nullptr);
    CMenuSaveGame_ResetStringStorage(record + 0x16C, nullptr);

    void* block = CSF_AT(void*, self, 0x110);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x110) = nullptr;
    }

    block = CSF_AT(void*, self, 0x100);
    if (block)
    {
        Mem_Free(block);
        CSF_AT(void*, self, 0x100) = nullptr;
    }

    DestroyPointerStorage(reinterpret_cast<int*>(record + 0xF0), nullptr);

    if (CSF_AT(uint32_t, self, 0xA8) >= 0x10u)
    {
        void* stringBuffer = CSF_AT(void*, self, 0x94);
        if (stringBuffer)
            Mem_FreeDebug(stringBuffer);
    }
    CSF_AT(uint32_t, self, 0xA8) = 15;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint8_t, self, 0x94) = 0;

    if (CSF_AT(uint32_t, self, 0x8C) >= 0x10u)
    {
        void* stringBuffer = CSF_AT(void*, self, 0x78);
        if (stringBuffer)
            Mem_FreeDebug(stringBuffer);
    }
    CSF_AT(uint32_t, self, 0x8C) = 15;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(uint8_t, self, 0x78) = 0;

    if (CSF_AT(uint32_t, self, 0x70) >= 0x10u)
    {
        void* stringBuffer = CSF_AT(void*, self, 0x5C);
        if (stringBuffer)
            Mem_FreeDebug(stringBuffer);
    }
    CSF_AT(uint32_t, self, 0x70) = 15;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint8_t, self, 0x5C) = 0;

    if (CSF_AT(uint32_t, self, 0x54) >= 0x10u)
    {
        void* stringBuffer = CSF_AT(void*, self, 0x40);
        if (stringBuffer)
            Mem_FreeDebug(stringBuffer);
    }
    CSF_AT(uint32_t, self, 0x54) = 15;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint8_t, self, 0x40) = 0;

    if (CSF_AT(uint32_t, self, 0x38) >= 0x10u)
    {
        void* stringBuffer = CSF_AT(void*, self, 0x24);
        if (stringBuffer)
            Mem_FreeDebug(stringBuffer);
    }
    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x24) = 0;

    return CMenuSaveGame_ResetStringStorage(record + 4, nullptr);
}

HOOK(0x00511040, GameBddLoader_DestroyCharacterRecord, "auto");
