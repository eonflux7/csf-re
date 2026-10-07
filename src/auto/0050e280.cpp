#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads a dword field (offset 0x4C) of the sub-object pointed to by this->field_007C.
static int __fastcall C_unk50E280_GetInnerField4C(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x7c);
    return CSF_AT(int, inner, 0x4c);
}

HOOK(0x50E280, C_unk50E280_GetInnerField4C, "auto");
