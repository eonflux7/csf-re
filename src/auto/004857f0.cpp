#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Callee bound by address: `CSF_FUNC(int, __fastcall, sub_481100, 0x00481100, (void* self, void* edx, int a2));`
CSF_FUNC(int, __fastcall, CCharWeaponFsm_PreForward, 0x00481100, (void* self, void* edx, int a2));

// Original ret 4: __thiscall method written as __fastcall with one stack argument (callee pops 4).
static int __fastcall CCharActionFsm_ProcessAndForwardToChildren(CCharActionFsm* self, void* /*edx*/, int a2)
{
    CCharWeaponFsm_PreForward(self, nullptr, a2);

    // Element count of the child vector: (end - begin) / 4, signed shift as in the original.
    int32_t childCount = 0;
    if (self->field_0078)
        childCount = static_cast<int32_t>(self->field_007c - self->field_0078) >> 2;

    // Forward to each child through vtable slot 0x34 (slot 13).
    for (uint32_t i = 0; i < static_cast<uint32_t>(childCount); ++i)
    {
        auto* element = reinterpret_cast<uint32_t*>(self->field_0078 + i * 4);
        void* child = reinterpret_cast<void*>(*element);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, child, 0)[13])(child, nullptr, a2);
    }

    if (self->field_0088)
    {
        void* secondary = reinterpret_cast<void*>(self->field_0088);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, secondary, 0)[13])(secondary, nullptr, a2);
    }

    int result = static_cast<int>(self->field_008c);
    if (result)
    {
        void* tail = reinterpret_cast<void*>(self->field_008c);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, tail, 0)[13])(tail, nullptr, a2);
    }
    return result;
}

HOOK(0x004857F0, CCharActionFsm_ProcessAndForwardToChildren, "auto");
