#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008344f0_ForwardSlot1CWithZeroFlag(C_008344f0* self, void* /*edx*/, int a2, int a3)
{
    // Original: mov eax,[ecx]; push a3; push 0; push a2; call [eax+1Ch]; ret 8.
    // eax is returned whole, so the slot is called with an int result (the
    // vSlot1C wrapper binds char and would truncate it).
    return reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int)>(
        self->vtbl[0x1c / 4])(self, nullptr, a2, 0, a3);
}

HOOK(0x004A2CB0, C_008344f0_ForwardSlot1CWithZeroFlag, "auto");
