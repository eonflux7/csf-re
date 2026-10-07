#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072F820 (7 bytes): trivial __thiscall getter returning the dword at offset 0x204.
//   mov eax, dword ptr [ecx + 0x204]
//   ret
static uint32_t __fastcall C_00844928_GetField0204(C_00844928* self, void* /*edx*/)
{
    return self->field_0204;
}

HOOK(0x0072F820, C_00844928_GetField0204, "auto");
