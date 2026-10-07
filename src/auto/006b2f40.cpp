// Target: 0x006B2F40 - forwards to the virtual method at vtable slot 0x70
// of the sub-object embedded at offset 0x30 of C_00860288.
// Original code performs a tail-jump (jmp [eax+0x70]) with this = self + 0x30.

#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Tail call into the sub-object's virtual method (slot 0x70 / 4 = 28).
static int __fastcall C_00860288_DispatchSubobjectVirtual70(C_00860288* self, void* /*edx*/)
{
    // add ecx, 0x30: the sub-object is embedded at +0x30 (its address, not a pointer stored there)
    void* subObject = reinterpret_cast<uint8_t*>(self) + 0x30;
    void** subObjectVtbl = *reinterpret_cast<void***>(subObject);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(subObjectVtbl[28]))(subObject, nullptr);
}

HOOK(0x6B2F40, C_00860288_DispatchSubobjectVirtual70, "auto");
