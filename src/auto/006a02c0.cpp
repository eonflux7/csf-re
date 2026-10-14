#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: bind as given by the packet.
CSF_FUNC(uint8_t*, __cdecl, sub_694BF0, 0x00694BF0, (uint8_t* a1, char a2, char a3));

// Copies a path string into a 200-byte stack buffer, rewrites every '/' (47)
// to '\' (92) through sub_694BF0, then copies the result into the destination.
// The original inlines its copy loops (no CRT strcpy call), so they are
// reproduced byte by byte here.
static int __cdecl ZlibLog_CopyPathBackslashes(char* dst, const char* src, int /*unused*/) {
    char local[0xC8];

    // Copy src into local, including the terminating zero.
    char* d = local;
    const char* s = src;
    while ((*d++ = *s++) != 0) {
    }

    sub_694BF0(reinterpret_cast<uint8_t*>(local), 47, 92);

    // Copy local back out to dst, including the terminating zero.
    d = dst;
    s = local;
    while ((*d++ = *s++) != 0) {
    }

    // The original never writes eax after its copy loop, so the returned
    // value is the register left by that loop: one past local's terminator.
    return static_cast<int>(reinterpret_cast<uintptr_t>(s));
}

HOOK(0x006A02C0, ZlibLog_CopyPathBackslashes, "auto");
