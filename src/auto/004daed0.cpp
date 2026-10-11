#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_IsSlotEmptyAndNotLocked, 0x0041D160, (void* self, void*));
CSF_FUNC(char, __fastcall, Autonomous_UpdateBase, 0x004D01C0, (void* self, void* edx, float a2));
CSF_FUNC(uint8_t, __fastcall, Autonomous_TryAdvance, 0x004DAB80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Autonomous_StateEnter2, 0x004DACC0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Autonomous_StateEnter3, 0x004DAD00, (void* self, void* edx, int a2));

static char __fastcall C_0083dd58_Update(C_0083dd58* self, void* /*edx*/, int a2)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014));

    if (self->field_0091) {
        void* target = CSF_AT(void*, owner, 0x44);
        void** vtbl = CSF_AT(void**, target, 0);
        int result = ((int(__fastcall*)(void*, void*, int))vtbl[0xBC / 4])(target, nullptr, 30);
        if (result != -1) {
            self->field_0092 = 1;
            self->field_0074 = 0;
        }
        self->field_0091 = 0;
    }

    if (!self->field_0038) {
        return 0;
    }

    float asFloat;
    std::memcpy(&asFloat, &a2, sizeof(float));
    Autonomous_UpdateBase(self, nullptr, asFloat);

    if (self->field_0070 == 2) {
        Autonomous_StateEnter2(self, nullptr, a2);
    } else if (self->field_0070 == 3) {
        Autonomous_StateEnter3(self, nullptr, a2);
    } else if (self->field_0070 == 4) {
        owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014));
        if (Vehicle_IsSlotEmptyAndNotLocked(CSF_AT(void*, owner, 0x4C), nullptr)) {
            self->field_0070 = 5;
            self->field_0018 &= ~1u;
        }
    }

    if (!self->field_0092) {
        return 0;
    }
    uint32_t pending = self->field_0074;
    if (pending) {
        return static_cast<char>(pending);
    }
    return static_cast<char>(Autonomous_TryAdvance(self, nullptr, a2));
}

HOOK(0x004DAED0, C_0083dd58_Update, "auto");
