#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(bool, __fastcall, Bounds_ContainsPoint, 0x006AC9A0, (uint8_t* self, void* , float* point));

static bool __fastcall IsPointInsideReturnedObjectBounds(
    void* self, void* /*edx*/, float* point)
{
    auto* dispatcher = CSF_AT(C_0082dd78*, self, 0x4C);
    if (!dispatcher)
        return false;

    auto* object = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(dispatcher, nullptr));
    if (!object)
        return false;

    if (!reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr))
        return false;

    auto* boundsSource = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x34 / 4])(object, nullptr);

    uint8_t bounds[0x3C];
    reinterpret_cast<void (__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, boundsSource, 0)[0x1C / 4])(
            boundsSource, nullptr, bounds);

    return Bounds_ContainsPoint(bounds, nullptr, point);
}

HOOK(0x0048E9D0, IsPointInsideReturnedObjectBounds, "auto");
