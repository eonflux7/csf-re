#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Calls the virtual deleting destructor (vtable slot 4, byte offset 0x10) on a
// ScheduleGroupBase object with the "free memory" flag set to 1.
// Returns the vtable call's eax result, or 0 when the pointer is null.
static int __cdecl DeleteScheduleGroupBase(int objectAddress) {
    if (objectAddress == 0) {
        return 0;
    }

    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(objectAddress)));
    auto virtualDelete = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[0x10 / 4]);
    return virtualDelete(obj, nullptr, 1);
}

HOOK(0x006FDB90, DeleteScheduleGroupBase, "auto");
