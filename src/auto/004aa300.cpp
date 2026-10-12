#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the object held in field_0004: calls its vtable slot +0x34
// with `this` = &field_0004 and one stack argument = &field_0008.
static int __fastcall C_0083aa80_ForwardSlot34ToHeldObject(C_0083aa80* self, void* /*edx*/)
{
    void* heldObject = reinterpret_cast<void*>(self->field_0004);
    auto slot34 = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(CSF_AT(void**, heldObject, 0)[0x34 / 4]);
    return slot34(&self->field_0004, nullptr, reinterpret_cast<uint8_t*>(self) + 0x08);
}

HOOK(0x004AA300, C_0083aa80_ForwardSlot34ToHeldObject, "auto");
