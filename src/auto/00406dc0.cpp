#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_DegreesToRadians, 0x0082D754);

static double __fastcall GetEntryFloatOrRadiansFromSource(void* self, void* /*edx*/, int index)
{
    void* ownerObj = CSF_AT(void*, self, 0x08);

    // ownerObj->vtbl[10]() -> sub-object
    void* levelObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, ownerObj, 0)[10])(ownerObj, nullptr);
    // levelObj->vtbl[6]() -> source object
    void* sourceObj = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, levelObj, 0)[6])(levelObj, nullptr);
    // sourceObj->vtbl[10]() -> angle in degrees
    double angleDegrees = ((double (__fastcall*)(void*, void*))CSF_AT(void**, sourceObj, 0)[10])(sourceObj, nullptr);

    double radians = angleDegrees * g_DegreesToRadians;

    uint8_t* entry = CSF_AT(uint8_t*, self, 0x0C) + 44 * index;
    if (CSF_AT(uint8_t, entry, 0))
    {
        if (CSF_AT(int32_t, entry, 0x20) != 2139095039)
            return static_cast<double>(CSF_AT(float, entry, 0x20));
    }
    return radians;
}

HOOK(0x406DC0, GetEntryFloatOrRadiansFromSource, "auto");
