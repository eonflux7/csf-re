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

// Callees of 0x0041AEC0
CSF_FUNC(char, __fastcall, Vehicle_UpdateWheelFloat, 0x0041ABF0, (void* self, void* edx, float a3));
CSF_FUNC(char, __fastcall, Vehicle_ProcessWheelInt, 0x0040F700, (void* self, void* edx, int a2));

static char __fastcall C_0082e1e8_ResetWheelState(C_0082e1e8* self, void* /*edx*/, int a2)
{
    char result = self->field_000c;
    if (result)
    {
        // Virtual call on the sub-object at +0x30, vtable slot 0x14 / 4 = 5, __thiscall with one stack arg.
        void* motor = reinterpret_cast<void*>(self->field_0030);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, motor, 0)[5])(motor, nullptr, a2);

        // Pass the stack value a2 reinterpreted as a float.
        float angleValue;
        std::memcpy(&angleValue, &a2, sizeof(float));
        Vehicle_UpdateWheelFloat(self, nullptr, angleValue);

        Vehicle_ProcessWheelInt(self, nullptr, a2);

        self->field_0086 = 0;

        CSF_AT(uint32_t, self, 0x20) = 0;
        CSF_AT(uint32_t, self, 0x24) = 0;
        CSF_AT(uint32_t, self, 0x28) = 0;
        CSF_AT(uint32_t, self, 0x2C) = 0;
        return 0;
    }
    return result;
}

HOOK(0x41AEC0, C_0082e1e8_ResetWheelState, "auto");
