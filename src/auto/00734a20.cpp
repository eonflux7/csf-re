#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00734A20 — returns the byte field at offset 0xFC of C_008670f0.
static char __fastcall C_008670f0_GetField00fc(C_008670f0* self, void* /*edx*/)
{
    return self->field_00fc;
}

HOOK(0x734A20, C_008670f0_GetField00fc, "auto");
