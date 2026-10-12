#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwImageRead, 0x006254C0, (int a1));
CSF_FUNC(bool, __cdecl, sub_705910, 0x00705910, (char* a1, int a2, int a3));
CSF_GLOBAL(uint32_t, g_ParticleReadParam, 0x0086103C);

// Original is __usercall: edx0 is an input register, a2 and a3 are stack arguments, ret 0.
// Declared __cdecl so the callee pops nothing, matching the original's ret 0.
// Fills up to a3 dwords at edx0 with image handles read from a2 (one per index 1..a3).
// Stops at the first failing step; returns true if at least one handle was stored.
static bool __cdecl ParticleRender_ReadImageBatch(void* edx0, char* a2, int a3) {
    alignas(8) uint8_t imageRecordBytes[0xC8];
    char* a1 = reinterpret_cast<char*>(imageRecordBytes);

    bool anyStored = false;
    std::memset(edx0, 0, static_cast<size_t>(4) * static_cast<size_t>(a3));

    int index = 1;
    uint32_t* outSlot = reinterpret_cast<uint32_t*>(edx0);
    do {
        // sub_705860: eax = a2, edx = a1, esi = index.
        // Pushes: 0x86103C first, then esi, so the top of stack (arg 1) is index.
        hook::Regs r{};
        r.eax = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a2));
        r.edx = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(a1));
        r.esi = static_cast<uint32_t>(index);
        uint32_t stack[] = {static_cast<uint32_t>(index), g_ParticleReadParam};
        hook::call_regs(CSF_ADDR_FN(0x00705860), &r, stack, 2);

        // The original pushes only a1 for sub_705910 and pops 12 bytes;
        // the two extra slots were never written by the original, so pass 0.
        if (!sub_705910(a1, 0, 0))
            break;

        int handle = RwImageRead(static_cast<int>(reinterpret_cast<uintptr_t>(a1)));
        if (handle == 0)
            break;

        *outSlot = static_cast<uint32_t>(handle);
        ++index;
        ++outSlot;
        anyStored = true;
    } while (index <= a3);

    return anyStored;
}

HOOK(0x00705990, ParticleRender_ReadImageBatch, "auto");
