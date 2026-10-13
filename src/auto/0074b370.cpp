#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards the call to virtual slot 2 (vtable offset 0x8) of the object
// passed as the sole stdcall argument, returning that slot's result.
static int __stdcall NetGhostDDL_ForwardSlot2Call(void* obj) {
    auto slot2 = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[2]);
    return slot2(obj, nullptr);
}

HOOK(0x74B370, NetGhostDDL_ForwardSlot2Call, "auto");
