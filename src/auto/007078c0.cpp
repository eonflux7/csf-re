#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: std::uint32_t *__thiscall sub_7078C0(C_008633a0 *this, std::uint32_t *a2), ret 4
static uint32_t* __fastcall C_008633a0_StopAndReadRootVector3(C_008633a0* self, void* /*edx*/, uint32_t* outVector) {
    // If an animation is active, stop it on the animator object at field_0008 (vtbl slot 0x24 = 36 / 4 = 9).
    if (self->field_0010 && self->field_0020) {
        void* animator = reinterpret_cast<void*>(self->field_0008);
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, animator, 0)[36 / 4])(animator, nullptr, 0);
        self->field_0020 = 0;
    }

    // Copy the three root values stored at offsets 0x30, 0x34, 0x38 of the hierarchy object at field_0018.
    uint8_t* hierarchy = reinterpret_cast<uint8_t*>(self->field_0018);
    outVector[0] = CSF_AT(uint32_t, hierarchy, 0x30);
    outVector[1] = CSF_AT(uint32_t, hierarchy, 0x34);
    outVector[2] = CSF_AT(uint32_t, hierarchy, 0x38);
    return outVector;
}

HOOK(0x007078C0, C_008633a0_StopAndReadRootVector3, "auto");
