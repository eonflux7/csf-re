#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IndexedChildForwardValue, 0x004084E0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(double, __stdcall, WheelContactFactor, 0x00418830, (float* a1, float* arg4, float* a3, float a4));
CSF_FUNC(int, __fastcall, PointBuffer_Grow, 0x00419920, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_WheelForceScale, 0x0082E264);

static void __fastcall C_0082e1e8_UpdateWheelForce(C_0082e1e8* self, void* /*edx*/, int a2)
{
    uint8_t* params = reinterpret_cast<uint8_t*>(a2);
    uint8_t* first;
    uint8_t* second;

    int idx = CSF_AT(int, params, 0x10);
    if (idx == -1)
        first = nullptr;
    else
        first = reinterpret_cast<uint8_t*>(self->field_0034) + 32 * idx;

    idx = CSF_AT(int, params, 0x14);
    if (idx == -1)
        second = nullptr;
    else
        second = reinterpret_cast<uint8_t*>(self->field_0044) + 32 * idx;

    if (!first) {
        idx = CSF_AT(int, params, 0x18);
        if (idx == -1)
            first = reinterpret_cast<uint8_t*>(self->field_0034);
        else
            first = reinterpret_cast<uint8_t*>(self->field_0034) + 32 * idx;
    }
    if (!second) {
        idx = CSF_AT(int, params, 0x1C);
        if (idx == -1)
            second = reinterpret_cast<uint8_t*>(self->field_0044);
        else
            second = reinterpret_cast<uint8_t*>(self->field_0044) + 32 * idx;
    }

    int pointA[3];
    int pointB[3];
    IndexedChildForwardValue(reinterpret_cast<void*>(self->field_0030), nullptr,
                             reinterpret_cast<int>(pointA), *reinterpret_cast<int*>(first));
    IndexedChildForwardValue(reinterpret_cast<void*>(self->field_0030), nullptr,
                             reinterpret_cast<int>(pointB), *reinterpret_cast<int*>(second));

    int obj = C_0082dd78_DispatchReturnedObject(reinterpret_cast<C_0082dd78*>(self), nullptr);
    int res = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(obj), 0)[0x30 / 4])(
        reinterpret_cast<void*>(obj), nullptr);
    float extra = CSF_AT(float, reinterpret_cast<void*>(res), 8);

    double factor = WheelContactFactor(reinterpret_cast<float*>(pointA),
                                       reinterpret_cast<float*>(pointB),
                                       reinterpret_cast<float*>(params + 0x38), extra);

    double scale;
    if (factor <= (double)g_fZero) {
        uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 44));
        scale = ((double)g_TransitionOne - (double)CSF_AT(float, params, 0x28)) *
                ((double)CSF_AT(float, inner, 476) * (double)g_WheelForceScale);
    } else {
        scale = ((double)self->field_0080 - (double)CSF_AT(float, params, 0x2C)) / (double)self->field_0080;
    }

    self->field_006c = (float)(scale * factor * (double)CSF_AT(float, params, 0) + (double)self->field_006c);

    if (self->field_006c <= g_fZero) {
        self->field_006c = 0.0f;
        return;
    }

    double force = (double)CSF_AT(float, params, 0) * (double)self->field_006c;
    int entry[7];
    entry[0] = pointA[0];
    entry[2] = pointA[2];
    CSF_AT(float, entry, 0x18) = (float)(-force);
    entry[3] = pointB[0];
    entry[5] = pointB[2];
    entry[1] = pointA[1];
    entry[4] = pointB[1];

    uint8_t* list = params + 0x4C;
    if (!(CSF_AT(uint32_t, list, 8) < CSF_AT(uint32_t, list, 4)))
        PointBuffer_Grow(list, nullptr, CSF_AT(int, list, 0xC));

    uint32_t count = CSF_AT(uint32_t, list, 8);
    uint8_t* dest = CSF_AT(uint8_t*, list, 0) + 28 * count;
    CSF_AT(uint32_t, list, 8) = count + 1;
    std::memcpy(dest, entry, 0x1C);
}

HOOK(0x0041A160, C_0082e1e8_UpdateWheelForce, "auto");
