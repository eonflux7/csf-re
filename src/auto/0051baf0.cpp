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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, C_0086c1e0_ResetBaseState, 0x0075B650, (void* self, void* edx));

// Cleanup for C_00840e38: restores its vtable, frees the owned buffer at +0x0C,
// clears the three owned fields, then tail-jumps into the base-class reset.
static int __fastcall C_00840e38_Cleanup(C_00840e38* self, void* /*edx*/) {
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00840E38);

    if (self->field_000c) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_000c));
    }
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;

    // Original: mov ecx, esi / pop esi / jmp 0x75b650 (tail call, result passes through)
    return C_0086c1e0_ResetBaseState(self, nullptr);
}

HOOK(0x0051BAF0, C_00840e38_Cleanup, "auto");
