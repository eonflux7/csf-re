#include <cstdint>

#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// Invokes a virtual slot (byte offset in the vtable) on obj with one stack
// argument. The original uses ecx = obj, one pushed argument, and callee cleanup
// (ret 4), which is reproduced by a __fastcall call with edx unused.
static int InvokeVirtualSlotWithArg(void* obj, uint32_t slotByteOffset, uint32_t arg) {
    void** vtable = CSF_AT(void**, obj, 0);
    auto method = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(vtable[slotByteOffset / 4]);
    return method(obj, nullptr, arg);
}

// Releases the child controls of a CMenuMpSessions-style menu in the original
// order: +0x74, +0x7C, +0x78, +0x24, +0x28, +0x2C, +0x30. Each non-null child
// gets a virtual call on slot 0x2A4 (or 0x2A0 for +0x7C) with its tag argument.
// The return value is the eax left by the last call (the one on +0x30), or 0
// when that child is null, as in the original.
static int __fastcall MenuMpSessions_ShutdownChildControls(void* self, void* /*edx*/) {
    void* child = CSF_AT(void*, self, 0x74);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3031);
    }

    child = CSF_AT(void*, self, 0x7c);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a0, 0x10);
    }

    child = CSF_AT(void*, self, 0x78);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3331);
    }

    child = CSF_AT(void*, self, 0x24);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3330);
    }

    child = CSF_AT(void*, self, 0x28);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3330);
    }

    child = CSF_AT(void*, self, 0x2c);
    if (child) {
        InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3330);
    }

    child = CSF_AT(void*, self, 0x30);
    if (child) {
        return InvokeVirtualSlotWithArg(child, 0x2a4, 0x636d3331);
    }
    return 0;
}

HOOK(0x005600A0, MenuMpSessions_ShutdownChildControls, "auto");
