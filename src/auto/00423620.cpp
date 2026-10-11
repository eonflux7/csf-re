#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, TaskState_422BF0, 0x00422BF0, (void* self, void* edx, float* arg4, char a3));
CSF_FUNC(int*, __fastcall, TaskState_422E40, 0x00422E40, (void* self, void* edx, void* a1, char a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, TaskState_48E5D0, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, TaskState_4DB140, 0x004DB140, (void* self, void* edx, int a2));

static int* __fastcall C_0082f9f8_StartTask(C_0082f9f8* self, void* /*edx*/, int* a2, char a3)
{
    uintptr_t result = 0;

    CSF_AT(uint8_t, self, 0x2B8) = 0;
    self->field_0018 &= 0xFFFFFFF9;

    uint32_t owner = CSF_AT(uint32_t, self, 0x10);
    if (owner) {
        void* obj = reinterpret_cast<void*>(owner);
        result = (uintptr_t)((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr);
        if (result == 7)
            return reinterpret_cast<int*>(result);
    }

    char ok = TaskState_48E5D0(reinterpret_cast<void*>(self->field_0014), nullptr, (int)(uintptr_t)self, 2, 1);
    result = (result & ~(uintptr_t)0xFF) | (uint8_t)ok;
    if (!ok)
        return reinterpret_cast<int*>(result);

    CSF_AT(uint32_t, self, 0x1FC) = 1;
    uint32_t* pos = reinterpret_cast<uint32_t*>(GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr));
    CSF_AT(uint32_t, self, 0x20C) = pos[0];
    CSF_AT(uint32_t, self, 0x210) = pos[1];
    CSF_AT(uint32_t, self, 0x214) = pos[2];
    CSF_AT(uint32_t, self, 0x218) = pos[0];
    CSF_AT(uint32_t, self, 0x21C) = pos[1];
    CSF_AT(uint32_t, self, 0x220) = pos[2];

    uint32_t mode = self->field_001c;
    if (mode == 0) {
        int sub = TaskState_4DB140(self, nullptr, (int)(uintptr_t)((uint8_t*)self + 0x100));
        return TaskState_422BF0(reinterpret_cast<void*>(sub), nullptr, reinterpret_cast<float*>(a2), a3);
    }
    if (mode - 1 == 0) {
        int sub = TaskState_4DB140(self, nullptr, (int)(uintptr_t)((uint8_t*)self + 0x100));
        return TaskState_422E40(reinterpret_cast<void*>(sub), nullptr, a2, a3);
    }
    return reinterpret_cast<int*>((uintptr_t)(mode - 1));
}

HOOK(0x00423620, C_0082f9f8_StartTask, "auto");
