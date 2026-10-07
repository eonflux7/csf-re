#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004448C0 (7 bytes)
//   mov eax, [ecx+4]
//   add eax, 8
//   ret
// __thiscall with no stack arguments (ret 0): returns the dword at this+4, offset by 8.
static int __fastcall GetBaseOffset8(void* self, void* /*edx*/)
{
    return static_cast<int>(CSF_AT(uint32_t, self, 0x4) + 8u);
}

HOOK(0x4448C0, GetBaseOffset8, "auto");
