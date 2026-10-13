#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Resets field_0024 to 0, then tail-jumps through the vtable of the object
// stored in field_0020 (vtbl slot 8, byte offset 0x20) with the caller's args.
static hook::Tail __fastcall C_00862640_ResetCounterAndForwardInner(void* self, const uint32_t* args) {
    (void)args;
    auto* owner = static_cast<C_00862640*>(self);

    owner->field_0024 = 0;

    void* inner = reinterpret_cast<void*>(owner->field_0020);
    return hook::tail_jump(inner, (*reinterpret_cast<void***>(inner))[0x20 / 4]);
}

HOOK_TAIL(0x6F2CA0, C_00862640_ResetCounterAndForwardInner, "game");
