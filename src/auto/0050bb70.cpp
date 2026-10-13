#include <cstdint>
#include "globals.h"
#include "hook.h"

// Runs the four optional child-menu-context slots in order (vtable slot 0, no args),
// then tail-jumps into the fourth child's slot 0, or returns 0 if it is absent.
static hook::Tail __fastcall OptionsFile_ProcessMenuContext(void* self, const uint32_t* args) {
    (void)args;

    // First child at +0x04: call its vtable slot 0.
    void* firstChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04));
    if (firstChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, firstChild, 0)[0])(firstChild, nullptr);
    }

    // Second child at +0x08.
    void* secondChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    if (secondChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, secondChild, 0)[0])(secondChild, nullptr);
    }

    // Third child at +0x0C.
    void* thirdChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C));
    if (thirdChild) {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, thirdChild, 0)[0])(thirdChild, nullptr);
    }

    // Fourth child at +0x10: its slot 0 is reached by tail jump, with the child as ecx.
    void* fourthChild = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (fourthChild) {
        return hook::tail_jump(fourthChild, (*reinterpret_cast<void***>(fourthChild))[0x00 / 4]);
    }

    // Absent fourth child: original returns with eax == 0.
    return hook::tail_ret(0);
}

HOOK_TAIL(0x0050BB70, OptionsFile_ProcessMenuContext, "options_file");
