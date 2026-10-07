#include <cstdint>
#include "hook.h"

// 0x004F5170: int __thiscall sub_4F5170(_DWORD **this)
//
// Loads the object pointer stored at this+4 and tail-jumps to that
// object's virtual method at vtable byte offset 0x20 (slot 8).
// No stack arguments are forwarded, so a plain virtual call is
// functionally identical to the original tail jump.
static int __fastcall InvokeMemberVirtualSlot8(void** self, void* /*edx*/)
{
    void* obj = self[1];
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        (*reinterpret_cast<void***>(obj))[8])(obj, nullptr);
}

HOOK(0x4F5170, InvokeMemberVirtualSlot8, "auto");
