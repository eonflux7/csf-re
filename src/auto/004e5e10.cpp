#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, TryTurnTowards, 0x004E5640, (void* self, void* edx, float angle, void* dir));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_MinDistanceSqEpsilon, 0x0083E9AC);

typedef int (__fastcall *QueryFn)(S_004e5640* self, void* edx, void* a, void* b);
typedef void (__fastcall *GetPosFn)(S_004e5640* self, void* edx, float* out);
typedef void (__fastcall *ApplyFn)(S_004e5640* self, void* edx, void* a);

static void __fastcall SteerTowardsObject(S_004e5640* self, void* /*edx*/, int* dirXZ, C_008342e8* target)
{
    if (reinterpret_cast<C_008342e8*>(self->field_0008) == target)
        return;

    alignas(8) uint8_t taskBytes[0x34];
    uint8_t* task = taskBytes;
    void** vtbl = reinterpret_cast<void**>(self->field_0000);

    CSF_AT(void*, task, 0x00) = CSF_VTBL(0x0083124C);
    CSF_AT(int, task, 0x0C) = -1;
    CSF_AT(int, task, 0x10) = -1;
    CSF_AT(void*, task, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, task, 0x18) = -1;
    CSF_AT(int, task, 0x1C) = -1;
    CSF_AT(int, task, 0x20) = 0;
    CSF_AT(int, task, 0x24) = 0;
    CSF_AT(int, task, 0x28) = 0;
    CSF_AT(int, task, 0x04) = -1;
    CSF_AT(int, task, 0x08) = -1;
    CSF_AT(uint32_t, task, 0x2C) = 0x7F7FFFFF;
    CSF_AT(int, task, 0x30) = 0;

    int found = ((QueryFn)vtbl[168 / 4])(self, nullptr, &target->field_001c, task);
    if (found == -1)
        return;

    vtbl = reinterpret_cast<void**>(self->field_0000);
    uint8_t* sceneSub = CSF_AT(uint8_t*, g_pCurrentScene, 0xB0);
    CSF_AT(int, task, 0x30) = CSF_AT(int, sceneSub, 8);

    alignas(4) uint8_t selfPosBytes[0xC];
    float* selfPos = reinterpret_cast<float*>(selfPosBytes);
    ((GetPosFn)vtbl[136 / 4])(self, nullptr, selfPos);

    float* targetPos = GetObjectPositionVector(target, nullptr);
    float dz = targetPos[2] - selfPos[2];
    float dy = targetPos[1] - selfPos[1];
    float dx = targetPos[0] - selfPos[0];

    alignas(4) uint8_t deltaBytes[0xC];
    float* delta = reinterpret_cast<float*>(deltaBytes);
    delta[0] = dx;
    delta[1] = dy;
    delta[2] = dz;

    bool apply = false;
    if (dz * dz + dx * dx <= g_MinDistanceSqEpsilon) {
        apply = true;
    } else {
        alignas(4) uint8_t dirBytes[0xC];
        int a0 = dirXZ[0];
        int a1 = dirXZ[1];
        CSF_AT(int, dirBytes, 0) = a0;
        CSF_AT(int, dirBytes, 8) = a1;
        CSF_AT(int, dirBytes, 4) = 0;

        if (CSF_AT(int, task, 0x08) == 14) {
            uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
            if ((CSF_AT(uint32_t, owner, 0x170) & 0x2000) != 0 && CSF_AT(int, owner, 0x1D8) == 3)
                apply = true;
        }
        if (!apply) {
            float angle = (float)S_00526d40_GetVectorAngleRadians(
                reinterpret_cast<S_00526d40*>(dirBytes), nullptr);
            if (TryTurnTowards(self, nullptr, angle, delta))
                apply = true;
        }
    }

    if (apply) {
        vtbl = reinterpret_cast<void**>(self->field_0000);
        ((ApplyFn)vtbl[180 / 4])(self, nullptr, task);
    }
}

HOOK(0x004E5E10, SteerTowardsObject, "auto");
