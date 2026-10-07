#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x007283B0  (ret 4)
// Setter for the owned object pointer at +0x30. If the incoming pointer differs
// from the currently stored one, the previously held object is released through
// its vtable slot 0 (invoked with argument 1), the field is zeroed, and the
// incoming pointer is then stored.
static int __fastcall C_008656d0_SetOwnedPointer0030(C_008656d0* self, void* /*edx*/, void* incomingObject)
{
    void* storedObject = reinterpret_cast<void*>(self->field_0030);
    int result; // undefined unless the release path runs, exactly as in the original

    if (incomingObject != storedObject)
    {
        if (storedObject)
        {
            void** vtbl = *reinterpret_cast<void***>(storedObject);
            result = ((int(__fastcall*)(void*, void*, int))vtbl[0])(storedObject, nullptr, 1);
            self->field_0030 = 0;
        }
        self->field_0030 = reinterpret_cast<uint32_t>(incomingObject);
    }
    return result;
}

HOOK(0x7283B0, C_008656d0_SetOwnedPointer0030, "auto");
