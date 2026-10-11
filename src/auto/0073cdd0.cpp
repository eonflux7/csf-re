#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_73CA40, 0x0073CA40, (void* self, void* edx));

static int __fastcall C_00873b14_GetVtableIfValid(C_00873b14* self, void* /*edx*/) {
    if (sub_73CA40(self, nullptr)) {
        // mov eax, [esi]: first dword of the object is its vtable pointer
        return static_cast<int>(CSF_AT(uint32_t, self, 0));
    }
    return 0;
}

HOOK(0x0073CDD0, C_00873b14_GetVtableIfValid, "auto");
