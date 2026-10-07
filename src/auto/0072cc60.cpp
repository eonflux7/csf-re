#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072CC60 (7 bytes) — returns C_00865e38::field_014c (dword at this+0x14c).
static int __fastcall C_00865e38_GetField014c(C_00865e38* self, void* /*edx*/)
{
    return (int)self->field_014c;
}

HOOK(0x0072CC60, C_00865e38_GetField014c, "auto");
