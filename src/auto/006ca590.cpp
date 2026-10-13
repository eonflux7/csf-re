#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Calls the vtable slot at +0x1c (slot 7) on the object stored at holder+4.
static void* GetHeldObjectViaSlot7(void* holder) {
    void* held = CSF_AT(void*, holder, 0x4);
    return ((void* (__fastcall*)(void*, void*))CSF_AT(void**, held, 0)[7])(held, nullptr);
}

static int __fastcall GetInnerFlagsStateCode(void* self, void* /*edx*/) {
    int resultCode = 2;

    void* firstObject = GetHeldObjectViaSlot7(self);
    if (CSF_AT(uint8_t, firstObject, 0x41)) {
        return 3;
    }

    void* secondObject = GetHeldObjectViaSlot7(self);
    if (CSF_AT(uint8_t, secondObject, 0x40)) {
        return 0;
    }

    return resultCode;
}

HOOK(0x6CA590, GetInnerFlagsStateCode, "auto");
