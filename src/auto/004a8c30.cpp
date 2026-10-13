#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d58c_SetAnglesFromPoint, 0x00405D20,
         (C_0082d58c* self, void* /*edx*/, float* point, int applyImmediately));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620,
         (C_008670f0* self, void* /*edx*/, int value));

// Message 42 (angles from point) and 43 (set field 0x0C) on C_00835140.
static void __fastcall C_00835140_HandleAimMessage(C_00835140* self, void* /*edx*/, int a2)
{
    // Resolve the skin target object: this->field_0014->field_004c->vGetSkinTargetObject()
    C_0082d58c* targetObject = nullptr;
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014));
    C_0082dd78* skinOwner = CSF_AT(C_0082dd78*, owner, 0x4C);
    if (skinOwner)
    {
        // Virtual call: vtbl slot 0x3C / 4 = 15, no typed wrapper for C_0082dd78.
        using GetSkinTargetFn = void* (__fastcall*)(void* self, void* edx);
        GetSkinTargetFn getSkinTarget =
            reinterpret_cast<GetSkinTargetFn>(CSF_AT(void**, skinOwner, 0)[15]);
        targetObject = reinterpret_cast<C_0082d58c*>(getSkinTarget(skinOwner, nullptr));
    }

    uint32_t* message = reinterpret_cast<uint32_t*>(a2);
    if (message[0] == 42)
    {
        self->field_0018 = self->field_0018 | 1u;
        C_0082d58c_SetAnglesFromPoint(targetObject, nullptr,
                                      reinterpret_cast<float*>(message + 1), 0);
        self->field_0040 = 1;
    }
    else if (message[0] == 43)
    {
        C_008670f0_SetField0C(reinterpret_cast<C_008670f0*>(targetObject), nullptr,
                              static_cast<int>(message[1]));
    }
}

HOOK(0x4A8C30, C_00835140_HandleAimMessage, "auto");
