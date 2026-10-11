#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches a task message: code 0 starts the task (sets bit 0 of field_0018,
// forwards to the owner's slot 0xAC with the payload, marks field_0070 = 1);
// code 0x26 (38) stops it (clears bit 0, sets field_0070 = 0, and forwards to slot 0xA0).
static int __fastcall C_0083ae28_HandleMessage(C_0083ae28* self, void* /*edx*/, int* message) {
    int result = *message;
    if (*message) {
        if (result == 0x26) {
            void* owner = reinterpret_cast<void*>(self->field_0014);
            void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x30));
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0xA0 / 4])(target, nullptr);
            self->field_0018 &= ~1u;
            self->field_0070 = 0;
        }
    } else {
        self->field_0018 |= 1u;
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x30));
        ((void(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, target, 0)[0xAC / 4])(
            target, nullptr, message + 1, 1);
        self->field_0070 = 1;
    }
    return result;
}

HOOK(0x4CEC00, C_0083ae28_HandleMessage, "auto");
