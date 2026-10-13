#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 64-byte block (16 dwords) from *sourcePtr into the buffer at self->field_0038,
// then returns the object's vtable slot address.
static void** __fastcall C_00862e88_CopyBlock64ToField(C_00862e88* self, void* /*edx*/, const void** sourcePtr)
{
    const void* source = *sourcePtr;
    std::memcpy(reinterpret_cast<void*>(self->field_0038), source, 0x40);
    return reinterpret_cast<void**>(&self->vtbl);
}

HOOK(0x6FC5E0, C_00862e88_CopyBlock64ToField, "auto");
