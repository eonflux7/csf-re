#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// NetCogible::<compare> (0x00742940, __thiscall, ret 4).
// Returns true when the argument equals the object's field at +0x004.
static bool __fastcall NetCogible_EqualsStoredField0004(NetCogible* self, void* /*edx*/, int value) {
    return value == static_cast<int>(self->field_0004);
}

HOOK(0x00742940, NetCogible_EqualsStoredField0004, "auto");
