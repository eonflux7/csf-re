#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Resets the primary HUD widget: notifies the object at +0xD0, then tail-jumps
// to slot 0x28 of the object at +0x30 (its handler receives the same stack args).
static hook::Tail __fastcall HudResetPrimaryWidget(void* self, const uint32_t* args) {
    (void)args;

    // First: call vtable slot 0x28 of the object at this+0xD0 (no arguments, result unused).
    void* notifyTarget = CSF_AT(void*, self, 0xD0);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, notifyTarget, 0)[0x28 / 4])(notifyTarget, nullptr);

    // Second: tail-jump to slot 0x28 of the object at this+0x30, with that object in ecx.
    void* tailObj = CSF_AT(void*, self, 0x30);
    return hook::tail_jump(tailObj, CSF_AT(void**, tailObj, 0)[0x28 / 4]);
}

HOOK_TAIL(0x438720, HudResetPrimaryWidget, "auto");
