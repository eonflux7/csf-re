#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Clears the two cached index fields, asks the object for its child (vtbl slot 0x28),
// and if the child accepts (vtbl slot 0xA8 returns true) tail-jumps into the child's
// vtbl slot 0xA4 with the caller's stack arguments untouched.
static hook::Tail __fastcall ResetIndicesAndForwardIfChildAccepts(void* self, const uint32_t* args) {
    (void)args;

    CSF_AT(int, self, 0x1AC) = -1;
    CSF_AT(int, self, 0x1B0) = -1;

    // result = self->vtbl[0x28 / 4](self)
    void* child = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
    if (child == nullptr) {
        return hook::tail_ret(0);
    }

    // accepted = child->vtbl[0xA8 / 4](child)
    bool accepted = ((bool (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0xA8 / 4])(child, nullptr);
    if (!accepted) {
        return hook::tail_ret(0);
    }

    // jmp child->vtbl[0xA4 / 4] with ecx = child
    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0xA4 / 4]);
}

HOOK_TAIL(0x0049DE80, ResetIndicesAndForwardIfChildAccepts, "game");
