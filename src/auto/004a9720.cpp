#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees of the original (bound by address).
CSF_FUNC(int, __fastcall, sub_4A8DE0, 0x004A8DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4A9140, 0x004A9140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4A9340, 0x004A9340, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));

// Sets the current state of the task (a2 is 1..4 selecting a handler) and dispatches to it.
// Original: __thiscall with one stack argument, `ret 4`.
static int __fastcall C_00835228_SetStateDispatch(C_00835228* self, void* /*edx*/, uint32_t newState)
{
    if (!self->field_0070)
    {
        // Notify the child object at field_0014 (its +0x44 sub-object) via vtable slot 0xA0 (+160 bytes).
        void* parent = reinterpret_cast<void*>(self->field_0014);
        void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, parent, 0x44));
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[160 / 4])(target, nullptr);
    }

    self->field_0070 = newState;

    int result = static_cast<int>(newState - 1);
    switch (newState)
    {
    case 1u:
        result = sub_4A8DE0(self, nullptr);
        break;
    case 2u:
        result = sub_4A9140(self, nullptr);
        break;
    case 3u:
        result = sub_4A9340(self, nullptr);
        break;
    case 4u:
        result = ResetState(reinterpret_cast<void*>(&self->field_001c), nullptr);
        self->field_0018 &= ~1u;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004A9720, C_00835228_SetStateDispatch, "auto");
