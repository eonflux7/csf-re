#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void* edx));

// Leaves the "state 1" (waiting) state: releases the child's resource through
// its vtable slot 0xA0 (the call the original makes with ecx only), moves the
// object to state 6, resets the embedded helper at +0x1C and clears bit 0 of
// the flags at +0x18. Returns the ResetState result when the state was 1,
// otherwise the value of (state - 1).
static int __fastcall C_0083c280_AdvanceFromStateOneToSix(C_0083c280* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0070 - 1);
    if (self->field_0070 == 1)
    {
        // owner = self->field_0014; child = owner->field_0030
        uint32_t childValue = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0014), 0x30);
        void* child = reinterpret_cast<void*>(childValue);

        // child->vtbl[0xA0 / 4]() with no stack arguments
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0xA0 / 4])(child, nullptr);

        self->field_0070 = 6;
        result = ResetState(reinterpret_cast<void*>(&self->field_001c), nullptr);
        self->field_0018 &= ~1u;
    }
    return result;
}

HOOK(0x004D62E0, C_0083c280_AdvanceFromStateOneToSix, "auto");
