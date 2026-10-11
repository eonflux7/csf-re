#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);

CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* /*edx*/));
CSF_FUNC(bool, __fastcall, Bounds_ContainsPoint, 0x006AC9A0, (uint8_t* self, void* , float* point));

// Takes the object's bounds, offsets them by g_MaxPercent, transforms the point through a2's
// vtable slot 0xA4 and tests whether the result lies inside the bounds.
static int __fastcall C_0083b958_IsPointInDispatchedBounds(C_0083b958* self, void* /*edx*/, int a2)
{
    alignas(8) uint8_t boundsBytes[0x3C];
    alignas(8) float pointBuf[3];

    void* owner = reinterpret_cast<void*>(self->field_0014);
    uint32_t dispatchInput = CSF_AT(uint32_t, owner, 0x4C);
    void* dispatched = C_0082dd78_DispatchReturnedObject(reinterpret_cast<void*>(dispatchInput), nullptr);

    auto getBoundsSource = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, dispatched, 0)[13]);
    void* boundsSource = getBoundsSource(dispatched, nullptr);

    auto fillBounds = reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(CSF_AT(void**, boundsSource, 0)[7]);
    fillBounds(boundsSource, nullptr, boundsBytes);

    pointBuf[0] = CSF_AT(float, boundsBytes, 0x0C) + g_MaxPercent;
    CSF_AT(float, boundsBytes, 0x0C) = pointBuf[0];
    pointBuf[1] = CSF_AT(float, boundsBytes, 0x10) + g_MaxPercent;
    CSF_AT(float, boundsBytes, 0x10) = pointBuf[1];
    pointBuf[2] = CSF_AT(float, boundsBytes, 0x14) + g_MaxPercent;
    CSF_AT(float, boundsBytes, 0x14) = pointBuf[2];

    void* target = reinterpret_cast<void*>(a2);
    auto transformPoint = reinterpret_cast<int(__fastcall*)(void*, void*, float*)>(CSF_AT(void**, target, 0)[41]);
    int transformedResult = transformPoint(target, nullptr, pointBuf);

    return Bounds_ContainsPoint(boundsBytes, nullptr, reinterpret_cast<float*>(transformedResult));
}

HOOK(0x004D1560, C_0083b958_IsPointInDispatchedBounds, "auto");
