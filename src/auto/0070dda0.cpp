#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_70D5C0, 0x0070D5C0, (void* self, void* edx));

// Calls the object's virtual slot at +0x28 (vtable index 10). If it returns
// non-zero, forwards to sub_70D5C0 with the same `this`; otherwise returns 0.
static char __fastcall S_006fdbb0_ProcessIfAccepted(S_006fdbb0* self, void* /*edx*/) {
    char result = ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[40 / 4])(self, nullptr);
    if (result) {
        return sub_70D5C0(self, nullptr);
    }
    return result;
}

HOOK(0x0070DDA0, S_006fdbb0_ProcessIfAccepted, "auto");
