#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable slot 0xB8 / 4 = 46 on the sub-object stored at +0x38. Its return value is
// tested with `test al, al`, so it is bound as bool.
static int __fastcall C_008342e8_PassesSecondCheck(C_008342e8* self, void* /*edx*/) {
    void* subObject = reinterpret_cast<void*>(self->field_0038);
    if (subObject == nullptr) {
        return 0;
    }
    bool passes = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0xB8 / 4])(subObject, nullptr);
    if (passes) {
        return 1;
    }
    return 0;
}

HOOK(0x0048E160, C_008342e8_PassesSecondCheck, "auto");
