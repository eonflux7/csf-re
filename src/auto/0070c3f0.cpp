#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 64-byte block (16 dwords) from *src into the buffer at self->field_0020,
// then returns the object's vtable slot address.
static void** __fastcall C_008637d8_CopyBlock64ToField(C_008637d8* self, void* /*edx*/, const void** sourcePtr)
{
    const void* source = *sourcePtr;
    std::memcpy(reinterpret_cast<void*>(self->field_0020), source, 0x40);
    return reinterpret_cast<void**>(&self->vtbl);
}

HOOK(0x70C3F0, C_008637d8_CopyBlock64ToField, "auto");
