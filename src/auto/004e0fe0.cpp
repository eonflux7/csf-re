#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

CSF_FUNC(uint32_t*, __cdecl, FindSetupValue, 0x004E5130,
         (int a1, int a2, int a3));
CSF_FUNC(int, __fastcall, C_0083e248_InitializeSetupValue, 0x004E2F50,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_0083e5e8_InitializeSetupValue, 0x004E2110,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_0083e418_InitializeSetupValue, 0x004E17F0,
         (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, C_0083e500_InitializeSetupValue, 0x004E1910,
         (void* self, void* edx, int a2, int a3));

static uint32_t* __cdecl CreateSetupValue(int type, int a2, int a3)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    if (CSF_AT(uint8_t, globalState, 0xD0) != 0)
    {
        uint32_t* existingValue = FindSetupValue(type, a2, a3);
        if (existingValue)
            return existingValue;
    }

    switch (type)
    {
    case 0:
    {
        void* storage = Mem_Alloc(76);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t*>(
            C_0083e248_InitializeSetupValue(storage, nullptr, a2, a3));
    }
    case 1:
    {
        void* storage = Mem_Alloc(52);
        if (!storage)
            return nullptr;
        return reinterpret_cast<uint32_t*>(
            C_0083e5e8_InitializeSetupValue(storage, nullptr, a2, a3));
    }
    case 2:
    {
        void* storage = Mem_Alloc(28);
        if (!storage)
            return nullptr;
        return C_0083e418_InitializeSetupValue(storage, nullptr, a2, a3);
    }
    case 7:
    {
        void* storage = Mem_Alloc(24);
        if (!storage)
            return nullptr;
        return C_0083e500_InitializeSetupValue(storage, nullptr, a2, a3);
    }
    default:
        return nullptr;
    }
}

HOOK(0x004E0FE0, CreateSetupValue, "auto");
