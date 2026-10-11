#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(bool, __fastcall, Vehicle_IsSlotEmptyAndNotLocked, 0x0041D160, (void* self, void*));
CSF_FUNC(char, __fastcall, C_0082e428_SetTargetPosition, 0x0041D830, (C_0082e428* self, void* , float* pos));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(uint8_t, __fastcall, C_0083dd58_RunMode3Handler, 0x004DAAE0, (void* self, void* edx));

// Sets the autonomous task state (1..5). Lazily initializes the owner's
// controller with mode 12 on first use, then dispatches to the state handler.
static uint8_t __fastcall C_0083dd58_SetState(C_0083dd58* self, void* /*edx*/, int newState)
{
    if (self->field_0070 == 0)
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
        void* controller = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x44));
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, controller, 0)[47]))(controller, nullptr, 12);
    }

    self->field_0070 = static_cast<uint32_t>(newState);
    uint8_t result = static_cast<uint8_t>(newState - 1);

    switch (newState)
    {
    case 1:
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
        void* modeObj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x30));
        result = ((uint8_t(__fastcall*)(void*, void*, uint32_t*, int))(CSF_AT(void**, modeObj, 0)[43]))(
            modeObj, nullptr, reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x84), 1);
        break;
    }
    case 2:
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
        void* vehicle = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
        result = Vehicle_IsSlotEmptyAndNotLocked(vehicle, nullptr);
        if (result)
        {
            result = static_cast<uint8_t>(C_0082e428_SetTargetPosition(
                reinterpret_cast<C_0082e428*>(vehicle), nullptr,
                reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x78)));
        }
        break;
    }
    case 3:
        result = C_0083dd58_RunMode3Handler(self, nullptr);
        break;
    case 5:
        result = static_cast<uint8_t>(ResetState(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr));
        self->field_0018 &= ~1u;
        break;
    default:
        break;
    }
    return result;
}

HOOK(0x004DAC20, C_0083dd58_SetState, "auto");
