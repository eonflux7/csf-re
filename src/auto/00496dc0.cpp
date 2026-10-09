#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

// Vtable slot 0xD8 / 4 = 54 on the object held in field_0044; returns a C_008319c0*.
using GetControlledObjectFn = C_008319c0* (__fastcall*)(void* self, void* edx);

static int __fastcall CharActionFsm_GetCharacterToggleState(CPlayerCharacter* self, void* /*edx*/)
{
    if ((self->field_02e0 & 0x40) == 0)
        return 0;

    void* controlledObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044));
    auto getControlledObject = reinterpret_cast<GetControlledObjectFn>(CSF_AT(void**, controlledObject, 0)[0xD8 / 4]);
    C_008319c0* subObject = getControlledObject(controlledObject, nullptr);
    if (subObject == nullptr)
        return 0;

    if (C_008319c0_GetSubObjectField74(subObject, nullptr) == 2 && self->field_0360 == 0)
        return 1;

    return 0;
}

HOOK(0x00496DC0, CharActionFsm_GetCharacterToggleState, "auto");
