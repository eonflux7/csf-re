#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00862028_Destroy(C_00862028* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00862028);

    if (self->field_0054)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0054));
        self->field_0054 = 0;
    }
    if (self->field_0044)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0044));
        self->field_0044 = 0;
    }

    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0086200C);
    self->field_0014 = 0;
    self->field_0020 = 1;
    self->field_0021 = 0;
    self->field_0040 = 1;

    int result = static_cast<int>(self->field_000c);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x00861FAC);
    *(void***)self = CSF_VTBL(0x00861FC8);
    return result;
}

HOOK(0x006ED110, C_00862028_Destroy, "auto");
