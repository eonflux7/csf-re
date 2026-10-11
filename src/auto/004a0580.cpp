#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Global object pointer used by the hud alert icon code (dword_8B0444).
CSF_GLOBAL(uint32_t, g_HudAlertIconState, 0x008B0444);

static int __fastcall HudAlertIcons_RefreshIconTextures(C_008343a4* self, void* /*edx*/) {
    uint8_t* state = reinterpret_cast<uint8_t*>(g_HudAlertIconState);

    // First stage: fill state->+0x1C once, when +0x14 is set and +0x1C is still empty.
    if (CSF_AT(uint32_t, state, 0x14) != 0 && CSF_AT(uint32_t, state, 0x1C) == 0) {
        uint32_t textAddress;
        if (CSF_AT(uint32_t, state, 0x18) < 0x10) {
            textAddress = reinterpret_cast<uint32_t>(state + 4);  // inline buffer
        } else {
            textAddress = CSF_AT(uint32_t, state, 0x04);          // heap pointer
        }
        void* renderer = reinterpret_cast<void*>(self->field_000c);
        CSF_AT(uint32_t, state, 0x1C) =
            ((uint32_t(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, renderer, 0)[31])(
                renderer, nullptr, textAddress);
    }

    // Second stage: refresh the two icon slots.
    uint32_t result = 0;
    for (uint32_t byteOffset = 0; byteOffset < 8; byteOffset += 4) {
        void* slot = reinterpret_cast<void*>(CSF_AT(uint32_t, CSF_ADDR_DATA(0x008B043C), byteOffset));

        result = CSF_AT(uint32_t, slot, 0x20);
        if (result != 0 && CSF_AT(uint32_t, slot, 0x00) != 0) {
            uint32_t textAddress;
            if (CSF_AT(uint32_t, slot, 0x24) < 0x10) {
                textAddress = reinterpret_cast<uint32_t>(reinterpret_cast<uint8_t*>(slot) + 0x10);
            } else {
                textAddress = CSF_AT(uint32_t, slot, 0x10);
            }
            void* renderer = reinterpret_cast<void*>(self->field_000c);
            result = ((uint32_t(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, renderer, 0)[31])(
                renderer, nullptr, textAddress);
            CSF_AT(uint32_t, slot, 0x00) = result;
        }
    }

    return static_cast<int>(result);
}

HOOK(0x004A0580, HudAlertIcons_RefreshIconTextures, "auto");
