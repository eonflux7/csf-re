#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls vtable slot 2 (no args) on four optional child objects at +0x38, +0x40, +0x3C,
// in that order, then tail-jumps into the +0x44 child's slot 2, or returns 0 if it is absent.
static hook::Tail __fastcall HudStringShow_NotifyChildSlotsAndTail(void* self, const uint32_t* args) {
    (void)args;

    // Child at +0x38: call its vtable slot 2.
    void* firstChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x38));
    if (firstChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, firstChild, 0)[0x08 / 4])(firstChild, nullptr);
    }

    // Child at +0x40.
    void* secondChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x40));
    if (secondChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, secondChild, 0)[0x08 / 4])(secondChild, nullptr);
    }

    // Child at +0x3C.
    void* thirdChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x3C));
    if (thirdChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, thirdChild, 0)[0x08 / 4])(thirdChild, nullptr);
    }

    // Child at +0x44: its slot 2 is reached by tail jump, with the child as ecx.
    void* fourthChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x44));
    if (fourthChild) {
        return hook::tail_jump(fourthChild, (*reinterpret_cast<void***>(fourthChild))[0x08 / 4]);
    }

    // Absent fourth child: original returns with eax == 0.
    return hook::tail_ret(0);
}

HOOK_TAIL(0x004F4010, HudStringShow_NotifyChildSlotsAndTail, "hud_string_show");
