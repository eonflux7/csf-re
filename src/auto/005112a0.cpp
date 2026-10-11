#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CMenuSaveGame_ResetStringStorage, 0x00544310, (void* self, void*));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall GameBddLoader_ResetStringAllocator(void* self, void* /*edx*/)
{
    auto* bytes = static_cast<std::uint8_t*>(self);

    DestroyPointerStorage(reinterpret_cast<int*>(bytes + 0x190), nullptr);
    DestroyPointerStorage(reinterpret_cast<int*>(bytes + 0x180), nullptr);
    DestroyPointerStorage(reinterpret_cast<int*>(bytes + 0x170), nullptr);
    DestroyPointerStorage(reinterpret_cast<int*>(bytes + 0x160), nullptr);
    DestroyPointerStorage(reinterpret_cast<int*>(bytes + 0x150), nullptr);

    if (CSF_AT(std::uint32_t, self, 0xB8) >= 0x10u)
    {
        void* block = CSF_AT(void*, self, 0xA4);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(std::uint32_t, self, 0xB8) = 15;
    CSF_AT(std::uint32_t, self, 0xB4) = 0;
    CSF_AT(std::uint8_t, self, 0xA4) = 0;

    if (CSF_AT(std::uint32_t, self, 0x9C) >= 0x10u)
    {
        void* block = CSF_AT(void*, self, 0x88);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(std::uint32_t, self, 0x9C) = 15;
    CSF_AT(std::uint32_t, self, 0x98) = 0;
    CSF_AT(std::uint8_t, self, 0x88) = 0;

    if (CSF_AT(std::uint32_t, self, 0x6C) >= 0x10u)
    {
        void* block = CSF_AT(void*, self, 0x58);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(std::uint32_t, self, 0x6C) = 15;
    CSF_AT(std::uint32_t, self, 0x68) = 0;
    CSF_AT(std::uint8_t, self, 0x58) = 0;

    if (CSF_AT(std::uint32_t, self, 0x50) >= 0x10u)
    {
        void* block = CSF_AT(void*, self, 0x3C);
        if (block)
            Mem_FreeDebug(block);
    }
    CSF_AT(std::uint32_t, self, 0x50) = 15;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint8_t, self, 0x3C) = 0;

    int result = CMenuSaveGame_ResetStringStorage(bytes + 0x1C, nullptr);
    if (CSF_AT(std::uint32_t, self, 0x18) >= 0x10u)
    {
        void* block = CSF_AT(void*, self, 0x04);
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(block));
        if (block)
            result = Mem_FreeDebug(block);
    }
    CSF_AT(std::uint32_t, self, 0x18) = 15;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint8_t, self, 0x04) = 0;
    return result;
}

HOOK(0x005112A0, GameBddLoader_ResetStringAllocator, "auto");
