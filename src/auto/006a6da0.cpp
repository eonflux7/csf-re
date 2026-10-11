#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free (__cdecl, pops nothing; caller cleans up).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-style teardown: restore this object's vtable, release the owned
// child object through its slot-0 virtual (with flag 1), and free this block
// if bit 0 of the flags argument is set. Returns this object as a dword pointer.
static uint32_t* __fastcall C_0085fe20_Destructor(C_0085fe20* self, void* /*edx*/, char flags) {
    self->vtbl = CSF_VTBL(0x0085FE20);

    void* ownedChild = reinterpret_cast<void*>(self->field_0004);
    if (ownedChild != nullptr) {
        // Virtual slot 0 of the child, called __thiscall with argument 1.
        using SlotZeroFn = void(__fastcall*)(void* self, void* edx, int release);
        SlotZeroFn releaseChild = reinterpret_cast<SlotZeroFn>(CSF_AT(void**, ownedChild, 0)[0]);
        releaseChild(ownedChild, nullptr, 1);
    }

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A6DA0, C_0085fe20_Destructor, "auto");
