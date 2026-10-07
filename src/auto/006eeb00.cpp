// Commandos: Strike Force — 0x006EEB00
// Tail-forwards to virtual method slot 5 of the object embedded at this+8.

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// The sub-object starting at this+8 is a __thiscall object whose vtable pointer
// lives at offset 8 (field_0008). The original reloads it and jumps through
// entry 5 (byte offset 0x14), passing this+8 as the new `this`.
static int __fastcall C_00862110_ForwardToEmbeddedObjectSlot5(C_00862110* self, void* /*edx*/)
{
    void* embedded = reinterpret_cast<uint8_t*>(self) + 8;
    void** vtbl = *reinterpret_cast<void***>(embedded);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(vtbl[5])(embedded, nullptr);
}

HOOK(0x006EEB00, C_00862110_ForwardToEmbeddedObjectSlot5, "auto");
