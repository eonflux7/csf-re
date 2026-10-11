#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, ConstructEntryTypeTwo, 0x00479350,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, ConstructEntryTypeOne, 0x0047A340,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, ConstructEntryTypeThree, 0x0047A6B0,
         (void* self, void* edx, int a2, int a3));

static uint32_t* __cdecl WeaponFirearms_CreateActionEntry(
    int type, int a2, int a3)
{
    void* storage;
    uint32_t* entry;

    switch (type)
    {
    case 1:
        storage = Mem_Alloc(140);
        if (!storage)
            return nullptr;
        entry = reinterpret_cast<uint32_t*>(
            ConstructEntryTypeOne(storage, nullptr, a2, a3));
        break;
    case 2:
        storage = Mem_Alloc(120);
        if (!storage)
            return nullptr;
        entry = ConstructEntryTypeTwo(storage, nullptr, a2, a3);
        break;
    case 3:
        storage = Mem_Alloc(100);
        if (!storage)
            return nullptr;
        entry = ConstructEntryTypeThree(storage, nullptr, a2, a3);
        break;
    default:
        return nullptr;
    }

    if (entry)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, entry, 0)[184 / 4])(entry, nullptr);
    }
    return entry;
}

HOOK(0x00475A40, WeaponFirearms_CreateActionEntry, "auto");
