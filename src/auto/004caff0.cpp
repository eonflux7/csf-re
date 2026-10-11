#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, CoverAttack_Prepare_4CA0F0, 0x004CA0F0, (void* self, void* edx, int a1));

// Returns the owner object (linked value minus 4), or null when there is no link.
static uint8_t* CoverAttack_GetOwner(int task)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(task), nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(linked - 4);
    return nullptr;
}

static int __fastcall CoverAttack_InitOwnerState(void* self, void* /*edx*/, int task)
{
    uint8_t* owner;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(uint8_t, owner, 0x6b0) = 0;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(int, owner, 0x6dc) = 0;

    owner = CoverAttack_GetOwner(task);
    CoverAttack_SetStance_492720(CSF_AT(void*, owner, 0x2c), nullptr, 3);

    owner = CoverAttack_GetOwner(task);
    {
        void* inner = CSF_AT(void*, owner, 0x2c);
        void** vtbl = CSF_AT(void**, inner, 0);
        ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8c / 4])(inner, nullptr, 0, 0, -1, 0);
    }

    CoverAttack_Prepare_4CA0F0(self, nullptr, task);

    owner = CoverAttack_GetOwner(task);
    CSF_AT(int, owner, 0x3b8) = 1;
    CSF_AT(int, owner, 0x3c0) = 0;
    CSF_AT(int, owner, 0x3c4) = 0;
    CSF_AT(uint32_t, owner, 0x3c0) = 0x3dcccccd;
    CSF_AT(uint32_t, owner, 0x3c4) = 0x3dcccccd;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(int, owner, 0x2f8) = 1;
    CSF_AT(int, owner, 0x300) = 0;
    CSF_AT(int, owner, 0x304) = 0;
    CSF_AT(int, owner, 0x30c) = 0;
    CSF_AT(int, owner, 0x308) = 3;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(uint32_t, owner, 0x300) = 0x3f000000;
    CSF_AT(uint32_t, owner, 0x304) = 0x3f000000;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(int, owner, 0x314) = 1;
    CSF_AT(int, owner, 0x31c) = 0;
    CSF_AT(int, owner, 0x320) = 0;
    CSF_AT(int, owner, 0x328) = 0;
    CSF_AT(int, owner, 0x324) = 3;

    owner = CoverAttack_GetOwner(task);
    CSF_AT(uint32_t, owner, 0x31c) = 0x3f000000;
    CSF_AT(uint32_t, owner, 0x320) = 0x3f000000;

    return 0x3f000000;
}

HOOK(0x004CAFF0, CoverAttack_InitOwnerState, "auto");
