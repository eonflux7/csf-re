#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(char*, __cdecl, sub_7613C0, 0x007613C0, (int a1));
CSF_FUNC(void*, __fastcall, sub_4AEF70, 0x004AEF70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, sub_4C5D60, 0x004C5D60, (void* self, void* edx, int a1));

// Returns the owner object (linked value - 4) or null.
static uint8_t* GetLinkedOwner(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(linked - 4);
    return nullptr;
}

static uint32_t __fastcall CoverAttackTask_ResetOwnerState(void* self, void* /*edx*/, int a1)
{
    uint8_t* owner = GetLinkedOwner(a1);
    uint8_t* inner = CSF_AT(uint8_t*, owner, 0x2c);
    uint8_t* mid = CSF_AT(uint8_t*, inner, 0x34);
    sub_7613C0(CSF_AT(int, mid, 0x2f4));

    owner = GetLinkedOwner(a1);
    sub_4AEF70(owner, nullptr);

    owner = GetLinkedOwner(a1);
    sub_492720(CSF_AT(void*, owner, 0x2c), nullptr, 3);

    owner = GetLinkedOwner(a1);
    void** obj = CSF_AT(void**, owner, 0x2c);
    void** vtbl = CSF_AT(void**, obj, 0);
    ((void(__fastcall*)(void*, void*, int, int, int, int))vtbl[0x8c / 4])(obj, nullptr, 0, 0, -1, 0);

    return sub_4C5D60(self, nullptr, a1);
}

HOOK(0x004C9F70, CoverAttackTask_ResetOwnerState, "auto");
