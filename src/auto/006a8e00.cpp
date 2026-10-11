#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Releases the object currently held in slot +0x04 (vtable slot 0 with arg 1,
// i.e. Release(1)) if one is present, then stores the new pointer in its place.
// Returns this.
static uint32_t* __fastcall ReleaseAndReplaceHeldPointer(uint32_t* self, void* /*edx*/, int newPointer)
{
    void* heldObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04));
    if (heldObject) {
        // vtable slot 0 (byte offset 0x00), called with arg 1
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, heldObject, 0)[0])(heldObject, nullptr, 1);
    }
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(newPointer);
    return self;
}

HOOK(0x006A8E00, ReleaseAndReplaceHeldPointer, "auto");
