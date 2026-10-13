#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the sub-object at +0x38 is non-null and its vtable slot 0xB4 / 4 = 45 reports true,
// otherwise 0. The slot's result is tested with `test al, al`, so it is bound as bool.
// The field is declared on C_00833908 (base of the 1052-byte player class), so that type is used here.
static int __fastcall CPlayerCharacter_PassesFirstCheck(C_00833908* self, void* /*edx*/) {
    uint32_t subObjectAddr = self->field_0038;
    if (subObjectAddr == 0) {
        return 0;
    }
    void* subObject = reinterpret_cast<void*>(subObjectAddr);
    bool passes = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, subObject, 0)[0xB4 / 4])(subObject, nullptr);
    if (passes) {
        return 1;
    }
    return 0;
}

HOOK(0x0048E120, CPlayerCharacter_PassesFirstCheck, "auto");
