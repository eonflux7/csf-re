#include "types/auto_classes.h"
#include "hook.h"

// Sets the object's identifier/handle dword at offset 0x10 to -1 (invalid).
static void __fastcall C_00862598_InvalidateField0010(C_00862598* self, void* /*edx*/)
{
    self->field_0010 = 0xFFFFFFFFu;
}

HOOK(0x006CA010, C_00862598_InvalidateField0010, "auto");
