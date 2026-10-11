#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, Object_ApplyAlternatePath, 0x00415690, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Object_ApplyDefaultPath, 0x00415770, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Object_UpdateFromEvent, 0x0041E090, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField400NonZero, 0x00495E70, (void* self, void*));

static char __fastcall C_0082dfd0_UpdateStateAndPosition(C_0082dfd0* self, void* /*edx*/, int a2)
{
    if (self->field_000c == 0)
        return 0;

    // Virtual slot +0x14 on the object held in field_0010 (first dword is its vtable).
    void* controller = reinterpret_cast<void*>(self->field_0010);
    using Slot14Fn = void(__fastcall*)(void*, void*, int);
    auto slot14 = reinterpret_cast<Slot14Fn>(CSF_AT(void**, controller, 0)[0x14 / 4]);
    slot14(controller, nullptr, a2);

    Object_UpdateFromEvent(self, nullptr, a2);

    void* object = reinterpret_cast<void*>(self->field_0004);
    self->field_008e |= static_cast<uint8_t>(IsField400NonZero(object, nullptr));

    if (CSF_AT(uint8_t, object, 0x228) != 0) {
        Object_ApplyAlternatePath(self, nullptr, a2);
    } else {
        if (CSF_AT(uint32_t, object, 0x50) != 0 ||
            (CSF_AT(uint32_t, object, 0x2E0) & 0x2000) == 0) {
            Object_ApplyDefaultPath(self, nullptr, a2);
        }
    }

    float* position = reinterpret_cast<float*>(GetObjectPositionVector(object, nullptr));
    CSF_AT(float, self, 0x18) = position[0];
    CSF_AT(float, self, 0x1C) = position[1];
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, position, 8);

    return static_cast<char>(CSF_AT(uint32_t, position, 8));
}

HOOK(0x00415860, C_0082dfd0_UpdateStateAndPosition, "auto");
