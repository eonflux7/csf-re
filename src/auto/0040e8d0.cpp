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

// Raw IEEE-754 bits of a float, as the original pushes them on the stack.
static uint32_t FloatBitsOf(float value) {
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082dd78_RefreshCachedState, 0x0040E170, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082dd78_ApplyDeltaStep, 0x0040E7D0, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* , int));

static char __fastcall C_0082dd78_UpdateIfEnabled(C_0082dd78* self, void* /*edx*/, float a2) {
    char result = static_cast<char>(self->field_000c);
    if (result) {
        if (!self->field_0060) {
            void* sourceObject = reinterpret_cast<void*>(self->field_0004);
            self->field_0060 = 1;
            void* positionVector = GetObjectPositionVector(sourceObject, nullptr);
            self->field_005c = CSF_AT(uint32_t, positionVector, 4);
        }

        C_0082dd78_RefreshCachedState(self, nullptr);

        void* motionObject = reinterpret_cast<void*>(self->field_0030);
        // Slot +0x14 of the object's vtable (slot 5), called with the raw bits of a2.
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, motionObject, 0)[5])(
            motionObject, nullptr, FloatBitsOf(a2));

        C_0082dd78_ApplyDeltaStep(self, nullptr, a2);
        C_0082e520_UpdateFromEvent(self, nullptr, static_cast<int>(FloatBitsOf(a2)));

        CSF_AT(uint32_t, self, 0x20) = 0;
        CSF_AT(uint32_t, self, 0x24) = 0;
        CSF_AT(uint32_t, self, 0x28) = 0;
        CSF_AT(uint32_t, self, 0x2C) = 0;
        return 0;
    }
    return result;
}

HOOK(0x0040E8D0, C_0082dd78_UpdateIfEnabled, "auto");
