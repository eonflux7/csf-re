#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets field_0024 to 0, calls vtbl slot 0x40 on the object at field_0020 (result discarded),
// then tail-jumps through that object's vtbl slot 0x20 with the caller's stack args forwarded.
static hook::Tail __fastcall C_00862490_ResetAndForwardToChild(void* self, const uint32_t* args) {
    (void)args;
    auto* owner = static_cast<C_00862490*>(self);

    owner->field_0024 = 0;

    // First virtual call: vtbl[0x40 / 4] on field_0020, no arguments.
    void* child = reinterpret_cast<void*>(owner->field_0020);
    ((void(__fastcall*)(void*, void*))(CSF_AT(void**, child, 0)[0x40 / 4]))(child, nullptr);

    // Original reloads field_0020 after the call before the tail jump.
    child = reinterpret_cast<void*>(owner->field_0020);
    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x20 / 4]);
}

HOOK_TAIL(0x6F1940, C_00862490_ResetAndForwardToChild, "auto");
