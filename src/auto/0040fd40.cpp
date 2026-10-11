#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Edge latch: returns 1 (and arms the latch) when the inner probe reports -1
// while the latch is clear; otherwise clears the latch and returns 0.
// Layout: byte +0 = latch flag, dword +4 = inner probe object.
// Inner probe vtable slot 0x48 / 4 = 18: int __thiscall (int arg, void* self).
static int __fastcall EdgeLatch_Poll(void* self, void* /*edx*/, int a2) {
    if (CSF_AT(uint8_t, self, 0x0) != 0) {
        CSF_AT(uint8_t, self, 0x0) = 0;
        return 0;
    }

    void* probe = CSF_AT(void*, self, 0x4);
    int probeResult =
        ((int(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, probe, 0)[0x48 / 4])(
            probe, nullptr, a2, self);

    if (probeResult == -1) {
        CSF_AT(uint8_t, self, 0x0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, self, 0x0) = 0;
    return 0;
}

HOOK(0x0040FD40, EdgeLatch_Poll, "auto");
