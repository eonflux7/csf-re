#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards a call to virtual slot 3 (byte offset 0x0C) of the object at a1,
// passing a2 as the single argument. The callee is __thiscall, so ecx carries
// a1 and edx is ignored; the original leaves a2 in edx, which is mirrored here.
static int __stdcall NetCogibleDdl_ForwardToVirtualSlot3(int a1, int a2) {
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
    auto slot3 = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[3]);
    return slot3(self, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), a2);
}

HOOK(0x00742000, NetCogibleDdl_ForwardToVirtualSlot3, "auto");
