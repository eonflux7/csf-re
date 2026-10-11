#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls slot 0xA0 of the object at [[self->field_0014] + 0x30]; if the state was 1,
// moves the state to 2 and clears bit 0 of field_0018.
static int __fastcall C_0083b850_AdvanceFromStateOne(C_0083b850* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0070) - 1;
    if (self->field_0070 == 1)
    {
        void* childObject = reinterpret_cast<void*>(self->field_0014);
        void* targetObject = reinterpret_cast<void*>(CSF_AT(uint32_t, childObject, 0x30));

        using SlotA0Fn = int(__fastcall*)(void* self, void* edx);
        SlotA0Fn slotA0 = reinterpret_cast<SlotA0Fn>(CSF_AT(void**, targetObject, 0)[0xA0 / 4]);
        result = slotA0(targetObject, nullptr);

        self->field_0070 = 2;
        self->field_0018 &= ~1u;
    }
    return result;
}

HOOK(0x004D1010, C_0083b850_AdvanceFromStateOne, "auto");
