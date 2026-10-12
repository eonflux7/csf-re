#include <cstdint>

#include "globals.h"
#include "hook.h"

// Calls the object's vtable slot 40 (byte offset 0xA0), then sets bits 0x6 in the dword at +0x18.
static int __fastcall ForwardSlot40AndSetFlagsAt18(void* self, void* /*edx*/) {
    using Slot40Fn = int(__fastcall*)(void* self, void* edx);
    Slot40Fn slot40 = reinterpret_cast<Slot40Fn>(CSF_AT(void**, self, 0)[0xA0 / 4]);
    int result = slot40(self, nullptr);
    CSF_AT(uint32_t, self, 0x18) |= 6u;
    return result;
}

HOOK(0x424080, ForwardSlot40AndSetFlagsAt18, "auto");
