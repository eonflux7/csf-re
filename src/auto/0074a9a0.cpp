#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_74A8B0, 0x0074A8B0, (void* self, void* edx));

static int __fastcall C_00873b14_GetVtableIfReady(C_00873b14* self, void* /*edx*/) {
    if (sub_74A8B0(self, nullptr)) {
        // mov eax, [esi]: first dword of the object is its vtable pointer
        return static_cast<int>(CSF_AT(uint32_t, self, 0));
    }
    return 0;
}

HOOK(0x0074A9A0, C_00873b14_GetVtableIfReady, "auto");
