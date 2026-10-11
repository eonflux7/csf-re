#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00835060_Construct, 0x004A8B40, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00835228_Construct, 0x004A9A80, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083ab58_Construct, 0x004CD690, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_0083ac40_Construct, 0x004CE6F0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083ad48_Construct, 0x004CE8C0, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, C_0083ae28_Construct, 0x004CEC80, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, C_0083af30_Construct, 0x004CEE00, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b038_Construct, 0x004CEEE0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b118_Construct, 0x004CEFB0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b1f8_Construct, 0x004CF340, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b2e0_Construct, 0x004CF4F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083b3c0_Construct, 0x004CF720, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b4a0_Construct, 0x004CFA20, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083b770_Construct, 0x004D0900, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, C_0083b850_Construct, 0x004D0E60, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083bb40_Construct, 0x004D3650, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083bc28_Construct, 0x004D3AD0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083c198_Construct, 0x004D50C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083c280_Construct, 0x004D6310, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083c388_Construct, 0x004D64B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083dbb0_Construct, 0x004D99F0, (C_0083dbb0* self, void* , uint32_t* owner));
CSF_FUNC(C_0083dd58*, __fastcall, C_0083dd58_Construct, 0x004DA950, (C_0083dd58* self, void* , int a2));
CSF_FUNC(uint32_t*, __cdecl, TryCreateAlternateCategoryObject, 0x004DC440, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static uint32_t** __cdecl CreateCategoryObject(int category, uint32_t* owner)
{
    const int ownerAddress = reinterpret_cast<int>(owner);

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        uint32_t* alternate = TryCreateAlternateCategoryObject(category, ownerAddress);
        if (alternate)
            return reinterpret_cast<uint32_t**>(alternate);
    }

    void* storage;
    switch (category)
    {
    case 1:
        storage = Mem_Alloc(832);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083dbb0_Construct(static_cast<C_0083dbb0*>(storage), nullptr, owner));

    case 2:
        storage = Mem_Alloc(148);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083dd58_Construct(static_cast<C_0083dd58*>(storage), nullptr, ownerAddress));

    case 3:
        storage = Mem_Alloc(232);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083c280_Construct(storage, nullptr, ownerAddress));

    case 4:
        storage = Mem_Alloc(232);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083ac40_Construct(storage, nullptr, ownerAddress));

    case 5:
        storage = Mem_Alloc(72);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b2e0_Construct(storage, nullptr, ownerAddress));

    case 6:
        storage = Mem_Alloc(68);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083c388_Construct(storage, nullptr, ownerAddress));

    case 8:
        storage = Mem_Alloc(64);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b038_Construct(storage, nullptr, ownerAddress));

    case 9:
        storage = Mem_Alloc(84);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b118_Construct(storage, nullptr, ownerAddress));

    case 10:
        storage = Mem_Alloc(80);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083ad48_Construct(storage, nullptr, ownerAddress));

    case 11:
        storage = Mem_Alloc(220);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_00835228_Construct(storage, nullptr, ownerAddress));

    case 13:
        storage = Mem_Alloc(1004);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083c198_Construct(storage, nullptr, ownerAddress));

    case 14:
        storage = Mem_Alloc(72);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b770_Construct(storage, nullptr, ownerAddress));

    case 15:
        storage = Mem_Alloc(116);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083ae28_Construct(storage, nullptr, ownerAddress));

    case 16:
        storage = Mem_Alloc(112);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083bc28_Construct(storage, nullptr, ownerAddress));

    case 17:
        storage = Mem_Alloc(76);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b1f8_Construct(storage, nullptr, ownerAddress));

    case 18:
        storage = Mem_Alloc(116);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083af30_Construct(storage, nullptr, ownerAddress));

    case 20:
        storage = Mem_Alloc(84);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b4a0_Construct(storage, nullptr, ownerAddress));

    case 21:
        storage = Mem_Alloc(72);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b3c0_Construct(storage, nullptr, ownerAddress));

    case 22:
        storage = Mem_Alloc(68);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_00835060_Construct(storage, nullptr, ownerAddress));

    case 23:
        storage = Mem_Alloc(68);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083bb40_Construct(storage, nullptr, ownerAddress));

    case 24:
        storage = Mem_Alloc(112);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083ab58_Construct(storage, nullptr, ownerAddress));

    case 25:
        storage = Mem_Alloc(128);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t**>(
            C_0083b850_Construct(storage, nullptr, ownerAddress));

    default:
        return nullptr;
    }
}

HOOK(0x004D1180, CreateCategoryObject, "auto");
