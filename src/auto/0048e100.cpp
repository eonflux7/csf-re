#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable slot 0xB0 / 4 = 44 on the sub-object stored at +0x38. Its return value is
// tested with `test al, al`, so it is bound as bool.
static int __fastcall C_00833b88_IsInactive(C_00833b88* self, void* /*edx*/) {
    void* subObject = reinterpret_cast<void*>(self->field_0038);
    if (subObject == nullptr) {
        return 0;
    }
    bool isInactive = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0xB0 / 4])(subObject, nullptr);
    if (isInactive) {
        return 1;
    }
    return 0;
}

HOOK(0x0048E100, C_00833b88_IsInactive, "auto");
