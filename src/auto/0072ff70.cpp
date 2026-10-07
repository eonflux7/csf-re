#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0072FF70 - returns the dword stored at this+0x138.
static uint32_t __fastcall C_00864bc0_GetField0138(C_00864bc0* self, void* /*edx*/)
{
    return self->field_0138;
}

HOOK(0x0072FF70, C_00864bc0_GetField0138, "auto");
