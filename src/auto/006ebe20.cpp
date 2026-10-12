#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00862028_ReleaseBufferAndResetFields(
    C_00862028* self, void* /*edx*/)
{
    uint32_t buffer = self->field_0054;
    self->field_0058 = 0;
    self->field_005c = 0;

    int result = 0;
    if (buffer) {
        result = Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(buffer)));
        self->field_0054 = 0;
    }
    return result;
}

HOOK(0x006EBE20, C_00862028_ReleaseBufferAndResetFields, "auto");
