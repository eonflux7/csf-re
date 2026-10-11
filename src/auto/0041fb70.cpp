#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x0048E4D0, (void* self, void* edx, int payload));
CSF_FUNC(int, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AngleScale, 0x0082E6C8);

static int FloatBits(float f)
{
    int bits;
    std::memcpy(&bits, &f, sizeof(bits));
    return bits;
}

static void CallSlot8C(void* obj)
{
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, obj, 0)[0x8C / 4])(obj, nullptr, 0, 0, -1, 0);
}

static int __fastcall C_0082e6e8_Update(C_0082e6e8* self, void* /*edx*/, float a2)
{
    void* obj = reinterpret_cast<void*>(self->field_0020);

    if ((uint8_t)SceneObj_IsInactive(obj, nullptr))
        return TCharTask_BaseUpdate(self, nullptr, FloatBits(a2));

    uint32_t state = self->field_003c;
    if (state == 0) {
        CallSlot8C(reinterpret_cast<void*>(self->field_0020));
        self->field_003c = 1;
        return TCharTask_BaseUpdate(self, nullptr, FloatBits(a2));
    }

    if (state == 1) {
        if (CSF_AT(float, self, 0x48) >= g_fZero) {
            float t = CSF_AT(float, self, 0x48) - a2;
            CSF_AT(float, self, 0x48) = t;
            if (t < g_fZero) {
                CallSlot8C(reinterpret_cast<void*>(self->field_0020));
            }
        }
        return TCharTask_BaseUpdate(self, nullptr, FloatBits(a2));
    }

    if (state != 2)
        return TCharTask_BaseUpdate(self, nullptr, FloatBits(a2));

    float step = a2 * self->field_0040;
    float* pos = reinterpret_cast<float*>(GetObjectPositionVector(obj, nullptr));
    float px = pos[0];
    float py = pos[1];
    float pz = pos[2];

    float newPos[3];
    newPos[0] = step * self->field_0030 + px;
    newPos[1] = step * self->field_0034 + py;
    newPos[2] = step * self->field_0038 + pz;

    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0020), 0x4C));
    ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, inner, 0)[0x50 / 4])(inner, nullptr, newPos);

    if ((self->field_0028 - newPos[1]) * self->field_0034
        + (self->field_0024 - newPos[0]) * self->field_0030
        + (self->field_002c - newPos[2]) * self->field_0038 < g_fZero) {
        self->field_003c = 1;
        self->field_0044 = 1;
        self->field_0048 = 0x3E4CCCCD;
    }

    float angle = (float)(S_00526d40_GetVectorAngleRadians(reinterpret_cast<S_00526d40*>(&self->field_0030), nullptr) * g_AngleScale);
    DispatchSceneObjectWithPayload(reinterpret_cast<void*>(self->field_0020), nullptr, FloatBits(angle));

    return TCharTask_BaseUpdate(self, nullptr, FloatBits(a2));
}

HOOK(0x0041FB70, C_0082e6e8_Update, "auto");
