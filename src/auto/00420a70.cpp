#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharTask_TryAcquire, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static char __fastcall C_0082f9f8_BeginTaskAtTarget(C_0082f9f8* self, void* /*edx*/, uint32_t* target, int param)
{
    int result;

    CSF_AT(uint8_t, self, 0x2B8) = 0;
    self->field_0018 &= 0xFFFFFFF9;

    uint32_t owner = CSF_AT(uint32_t, self, 0x10);
    bool skip = false;
    result = 0;
    if (owner != 0) {
        void** ownerObj = reinterpret_cast<void**>(owner);
        void** vtbl = CSF_AT(void**, ownerObj, 0);
        result = ((int(__fastcall*)(void*, void*))vtbl[0x80 / 4])(ownerObj, nullptr);
        if (result == 7)
            skip = true;
    }

    if (!skip) {
        result = (unsigned char)CharTask_TryAcquire(reinterpret_cast<void*>(self->field_0014), nullptr,
                                                    reinterpret_cast<int>(self), 2, 1);
        if ((char)result != 0) {
            CSF_AT(uint32_t, self, 0x1FC) = 1;
            uint32_t* pos = reinterpret_cast<uint32_t*>(
                GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr));

            CSF_AT(uint32_t, self, 0x20C) = pos[0];
            CSF_AT(uint32_t, self, 0x210) = pos[1];
            CSF_AT(uint32_t, self, 0x214) = pos[2];

            CSF_AT(uint32_t, self, 0x218) = pos[0];
            CSF_AT(uint32_t, self, 0x21C) = pos[1];
            CSF_AT(uint32_t, self, 0x220) = pos[2];

            CSF_AT(uint32_t, self, 0x224) = target[0];
            CSF_AT(uint32_t, self, 0x228) = target[1];
            CSF_AT(uint32_t, self, 0x22C) = target[2];

            CSF_AT(uint8_t, self, 0x205) = 1;
            CSF_AT(int, self, 0x208) = param;
            CSF_AT(uint8_t, self, 0x230) = 0;

            result = CharTask_StartMovement(self, nullptr,
                                            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0xB0));
            self->field_0018 |= 2u;
        }
    }
    return (char)result;
}

HOOK(0x00420A70, C_0082f9f8_BeginTaskAtTarget, "auto");
