#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Calls vtable slot 1 (byte offset 4) of the object with argument 1 when the
// object pointer is non-null. Returns the slot's result, or 0 for a null object
// (the original returns an uninitialized eax there; 0 is the garbage-free choice).
static int __stdcall GfxCameraProjection_InvokeSlotOneIfValid(int projectionObject)
{
    if (projectionObject != 0) {
        void* obj = reinterpret_cast<void*>(projectionObject);
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[1])(obj, nullptr, 1);
    }
    return 0;
}

HOOK(0x6F5B00, GfxCameraProjection_InvokeSlotOneIfValid, "auto");
