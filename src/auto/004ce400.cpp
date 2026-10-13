#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsField0050Null, 0x0040DA80, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0083ac40_BuildMovementPath, 0x004D5940, (C_0083ac40* self, void*));

static int __fastcall C_0083ac40_UpdateMovementState(C_0083ac40* self, void* /*edx*/, int a2) {
    (void)a2;

    C_0083b958* movementOwner = reinterpret_cast<C_0083b958*>(self->field_0014);
    void* ownerField4c = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, movementOwner, 0x4c)));

    int result = IsField0050Null(ownerField4c, nullptr);
    if (static_cast<uint8_t>(result) == 0) {
        return result;
    }

    uint32_t state = self->field_0070;
    if (state == 0) {
        void* stateObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, movementOwner, 0x44)));
        auto notify = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, stateObject, 0)[47]);
        notify(stateObject, nullptr, 14);
    } else if (state == 1) {
        void* pathObject = reinterpret_cast<void*>(static_cast<uintptr_t>(movementOwner->field_0030));
        auto stepPath = reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, pathObject, 0)[40]);
        stepPath(pathObject, nullptr);
    }

    self->field_0070 = 3;
    return C_0083ac40_BuildMovementPath(self, nullptr);
}

HOOK(0x004CE400, C_0083ac40_UpdateMovementState, "auto");
