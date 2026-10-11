#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(bool, __fastcall, Vehicle_IsSlotEmptyAndNotLocked, 0x0041D160, (void* self, void*));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

// Slot 0xBC / 4 = 47 on the object at owner->field_0044 (type not declared).
static void __fastcall C_0083dd58_CallSlot47OnOwnerObject(void* obj, void* /*edx*/, int arg)
{
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[47])(obj, nullptr, arg);
}

// __thiscall, 1 stack arg (a2, unused), callee pops 4.
static char __fastcall C_0083dd58_UpdateWhenSlotEmpty(C_0083dd58* self, void* /*edx*/, int a2)
{
    (void)a2;
    char result;

    auto* owner = reinterpret_cast<C_0083b958*>(self->field_0014);
    void* slotObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x4C)));
    result = Vehicle_IsSlotEmptyAndNotLocked(slotObj, nullptr) ? 1 : 0;

    if (result)
    {
        if (self->field_0070 == 0)
        {
            void* ownerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x44)));
            C_0083dd58_CallSlot47OnOwnerObject(ownerObj, nullptr, 12);
        }
        self->field_0070 = 5;
        result = static_cast<char>(ResetState(&self->field_001c, nullptr));
        self->field_0018 &= ~1u;
    }
    return result;
}

HOOK(0x004DACC0, C_0083dd58_UpdateWhenSlotEmpty, "auto");
