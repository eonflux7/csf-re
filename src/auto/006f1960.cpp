#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x006F1960. Loads the object pointer stored at this+0x20, calls its
// vtable slot 0x14 (no arguments) to obtain a second object, then tail-jumps into
// that object's vtable slot 0x10 with the second object in ecx. The caller's stack
// arguments and return address are forwarded untouched by hook::tail_jump.
static hook::Tail __fastcall C_00862640_ForwardThroughResolvedObject(void* self, const uint32_t* args) {
    (void)args;
    C_00862640* owner = reinterpret_cast<C_00862640*>(self);

    void* heldObject = reinterpret_cast<void*>(owner->field_0020);
    void** heldVtable = CSF_AT(void**, heldObject, 0);
    void* resolvedObject = reinterpret_cast<void*(__fastcall*)(void*, void*)>(heldVtable[0x14 / 4])(heldObject, nullptr);

    return hook::tail_jump(resolvedObject, (*reinterpret_cast<void***>(resolvedObject))[0x10 / 4]);
}

HOOK_TAIL(0x6F1960, C_00862640_ForwardThroughResolvedObject, "auto");
