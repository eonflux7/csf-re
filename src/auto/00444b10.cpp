#include <cstdint>
#include "hook.h"
#include "core/memory.h"

// 0x00444B10 (__thiscall, ret 4)
// Increments the dword counter at offset 0x50 of the object. The single
// stack argument is unused by the original code.
static void __fastcall C_444b10_IncrementCounter_0x50(void* self, void* /*edx*/, int /*unused*/)
{
    ++CSF_AT(uint32_t, self, 0x50);
}

HOOK(0x444B10, C_444b10_IncrementCounter_0x50, "auto");
