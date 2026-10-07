#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00724790 (7 bytes) — returns the dword stored at this+0x218.
// Disassembly:
//   mov eax, dword ptr [ecx + 0x218]
//   ret
static int __fastcall CGuiListBox_GetField0218(CGuiListBox* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0218);
}

HOOK(0x724790, CGuiListBox_GetField0218, "auto");
