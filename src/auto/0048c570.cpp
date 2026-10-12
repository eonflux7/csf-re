#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, CCharWeaponFsm_Construct, 0x00481E80, (CCharWeaponFsm* self, void* , std::uint32_t* owner));
CSF_FUNC(int, __fastcall, CCharActionFsm_Construct, 0x004891E0, (CCharActionFsm* self, void* , uint32_t* owner));
CSF_FUNC(uint32_t*, __fastcall, C_00833708_Construct, 0x0048A190, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Setup_InitializeActionSupport, 0x0048D7B0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static std::uint32_t* __cdecl Setup_CreateObject(
    int setupType, std::uint32_t* owner)
{
    switch (setupType)
    {
    case 0:
    {
        auto* weaponFsm = static_cast<CCharWeaponFsm*>(Mem_Alloc(116));
        if (weaponFsm)
            return CCharWeaponFsm_Construct(weaponFsm, nullptr, owner);
        return nullptr;
    }

    case 1:
    {
        void* setup = Mem_Alloc(28);
        if (setup)
            return C_00833708_Construct(
                setup, nullptr, reinterpret_cast<int>(owner));
        return nullptr;
    }

    case 2:
    {
        auto* storage = static_cast<CCharActionFsm*>(Mem_Alloc(216));
        int actionAddress = 0;
        if (storage)
            actionAddress = CCharActionFsm_Construct(storage, nullptr, owner);

        void* support = Mem_Alloc(52);
        if (support)
        {
            CSF_AT(void**, support, 0x10) = CSF_VTBL(0x0082D448);
            CSF_AT(int, support, 0x14) = -1;
            CSF_AT(int, support, 0x18) = -1;
            CSF_AT(void**, support, 0x24) = CSF_VTBL(0x0082D448);
            CSF_AT(int, support, 0x28) = -1;
            CSF_AT(int, support, 0x2C) = -1;
        }

        Setup_InitializeActionSupport(support, nullptr, actionAddress);

        auto* actionFsm = reinterpret_cast<CCharActionFsm*>(actionAddress);
        actionFsm->field_00d4 = reinterpret_cast<std::uint32_t>(support);
        return reinterpret_cast<std::uint32_t*>(actionFsm);
    }

    default:
        return nullptr;
    }
}

HOOK(0x0048C570, Setup_CreateObject, "auto");
