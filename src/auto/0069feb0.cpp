#include <cstdint>
#include "globals.h"
#include "hook.h"

// Gated forwarder: when the object's state (+0x20) is 2, it marks the state as 3, invokes
// vtable slot 2 of the sub-object at +0x40 with (a2, a3), restores state 2, refreshes the
// cached value at +0x08 from vtable slot 4 of the same sub-object, clears +0x0C, and returns
// the slot-2 result. Otherwise it returns 0 without side effects.
static int __cdecl RtFS_RunStateGatedOperation(void* a1, int a2, int a3) {
    uint8_t* self = static_cast<uint8_t*>(a1);
    int result = 0;

    if (CSF_AT(int, self, 0x20) == 2) {
        void* operationTarget = CSF_AT(void*, self, 0x40);
        CSF_AT(int, self, 0x20) = 3;
        int operationResult =
            ((int (__fastcall*)(void*, void*, int, int))CSF_AT(void**, operationTarget, 0)[2])(
                operationTarget, nullptr, a2, a3);

        void* refreshTarget = CSF_AT(void*, self, 0x40);
        CSF_AT(int, self, 0x20) = 2;
        CSF_AT(int, self, 0x08) =
            ((int (__fastcall*)(void*, void*))CSF_AT(void**, refreshTarget, 0)[4])(refreshTarget, nullptr);
        CSF_AT(int, self, 0x0C) = 0;
        return operationResult;
    }

    return result;
}

HOOK(0x69FEB0, RtFS_RunStateGatedOperation, "auto");
