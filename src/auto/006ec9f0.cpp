#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall C_00862028_ResetIntersectionBuffers(
    C_00862028* self, void* /*edx*/)
{
    const uint32_t firstBuffer = self->field_000c;
    self->field_0010 = 0;
    self->field_0014 = 0;
    if (firstBuffer)
    {
        Mem_Free(reinterpret_cast<void*>(firstBuffer));
        self->field_000c = 0;
    }

    self->field_0020 = 1;
    self->field_0021 = 0;
    self->field_0040 = 1;

    void* result = reinterpret_cast<void*>(self->field_0044);
    self->field_0048 = 0;
    self->field_004c = 0;
    if (result)
    {
        result = reinterpret_cast<void*>(Mem_Free(result));
        self->field_0044 = 0;
    }
    return result;
}

HOOK(0x006EC9F0, C_00862028_ResetIntersectionBuffers, "auto");
