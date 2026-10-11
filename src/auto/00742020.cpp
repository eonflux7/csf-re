#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards a call to virtual slot 11 (byte offset 0x2C) of the object at a1,
// passing a2 as the single argument. The callee is __thiscall, so ecx carries
// a1 and edx is ignored; the original leaves a2 in edx, which is mirrored here.
static int __stdcall NetCogibleDdl_ForwardToVirtualSlot11(int a1, int a2) {
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    auto slot11 = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[11]);
    return slot11(self, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), a2);
}

HOOK(0x00742020, NetCogibleDdl_ForwardToVirtualSlot11, "auto");
