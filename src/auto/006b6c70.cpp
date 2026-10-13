#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Keeps the candidate with the smaller key: if a4 < out->key (out+8),
// writes {value = a2->+4 + a3->+0x18 * 8, owner = a2} into out and returns a3.
static int __cdecl GfxWorld_KeepNearerCandidate(int a1, int a2, int a3, float a4, int a5)
{
    (void)a1;
    int result = a3;
    void* out = reinterpret_cast<void*>(a5);
    if (a4 < CSF_AT(float, out, 8))
    {
        uint32_t baseAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(a2), 4);
        uint32_t stride = static_cast<uint32_t>(CSF_AT(int, reinterpret_cast<void*>(a3), 0x18));
        uint32_t v6 = baseAddr + stride * 8;
        CSF_AT(uint32_t, out, 4) = static_cast<uint32_t>(a2);
        CSF_AT(uint32_t, out, 0) = v6;
        CSF_AT(float, out, 8) = a4;
    }
    return result;
}

HOOK(0x6B6C70, GfxWorld_KeepNearerCandidate, "auto");
