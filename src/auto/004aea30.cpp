#include <cstdint>
#include "globals.h"
#include "hook.h"

// CoreMemory_Free (0x00691110): __cdecl, bound as specified in the packet.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destroys the three owned sub-allocations of an AI task object and nulls their slots.
// Slot +0x20: embedded object with a header dword at -4 (nonzero => call its slot-0 deleting
// method with flag 3; zero => free the raw block at -4). Slot +0x10 and slot +0x00: plain blocks.
// Returns the value of the +0x00 slot as seen before the free (or the free's result if it was set).
static int __fastcall AiCore_DestroyEmbeddedTask(int* self, void* /*edx*/) {
    // First: embedded object at +0x20.
    uintptr_t embedded = static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20));
    if (embedded != 0) {
        void* obj = reinterpret_cast<void*>(embedded);
        if (CSF_AT(int, obj, -4) != 0) {
            // Virtual slot 0 (scalar deleting destructor), thiscall, one stack arg = 3.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0])(obj, nullptr, 3);
        } else {
            CoreMemory_Free(reinterpret_cast<void*>(embedded - 4));
        }
        CSF_AT(uint32_t, self, 0x20) = 0;
    }

    // Second: plain block at +0x10.
    uintptr_t secondBlock = static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x10));
    if (secondBlock != 0) {
        CoreMemory_Free(reinterpret_cast<void*>(secondBlock));
        CSF_AT(uint32_t, self, 0x10) = 0;
    }

    // Third: block at +0x00 (also the return value).
    uint32_t head = CSF_AT(uint32_t, self, 0);
    int result = static_cast<int>(head);
    if (head != 0) {
        result = CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(head)));
        CSF_AT(uint32_t, self, 0) = 0;
    }
    return result;
}

HOOK(0x004AEA30, AiCore_DestroyEmbeddedTask, "auto");
