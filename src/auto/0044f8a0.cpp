// Target: 0x0044F8A0 — sets the dword field at offset 0x1A4 to -1 (sentinel / invalid index).
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// __thiscall (no stack args, ret 0): ecx = self, edx unused.
static void __fastcall SetCurrentIndexInvalid(void* self, void* /*edx*/)
{
    // mov dword ptr [ecx + 0x1a4], 0xffffffff
    CSF_AT(int32_t, self, 0x1A4) = -1;
}

HOOK(0x0044F8A0, SetCurrentIndexInvalid, "auto");
