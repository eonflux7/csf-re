#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards to the child object stored at +0x10 through vtable slot 0x80 (32).
// Returns 0 when there is no child; otherwise returns the slot's result.
static int __fastcall C_0082f9f8_ForwardToChildOrZero(C_0082f9f8* self, void* /*edx*/, int /*a2*/) {
    int result = CSF_AT(int, self, 0x10);
    if (result) {
        void* child = reinterpret_cast<void*>(result);
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x80 / 4])(child, nullptr);
    }
    return result;
}

HOOK(0x421390, C_0082f9f8_ForwardToChildOrZero, "auto");
