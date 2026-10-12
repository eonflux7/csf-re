#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Handle_IsValid: __fastcall(self in ecx, edx unused), returns BOOL.
CSF_FUNC(int, __fastcall, Handle_IsValid, 0x006E5670, (void* self, void* edx));

// Original: mov ecx, [ecx+4]; jmp Handle_IsValid
// Tail call: the handle stored at +4 becomes Handle_IsValid's `this`.
static int __fastcall GfxObjectInstances_IsHandleValid(int* self, void* /*edx*/) {
    return Handle_IsValid(CSF_AT(void*, self, 0x4), nullptr);
}

HOOK(0x006E5FE0, GfxObjectInstances_IsHandleValid, "auto");
