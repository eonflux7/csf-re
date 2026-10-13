#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Original is __usercall: edx0 is the edx input register, a2/a3 are the caller's
// two stack arguments (ret 0). 0x0086103C is pushed as an ADDRESS (a string that
// both callees strlen), not as the dword's value.
CSF_FUNC(uint32_t, __cdecl, ParticleRender_CheckImagePageAvailable, 0x00705910,
         (const char* a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RwImageRead, 0x006254C0, (int a1));

// Fills a3 dwords at edx0 with image handles read from a2 (one per index 1..a3).
// Stops at the first failing step; returns whether any handle was stored (al).
static uint32_t __cdecl ParticleRender_ReadImageBatch(hook::Regs* regs, const uint32_t* args)
{
    void* edx0 = reinterpret_cast<void*>(regs->edx);
    const int a2 = static_cast<int>(args[0]);
    const int a3 = static_cast<int>(args[1]);
    const uint32_t paramAddr = CSF_ADDR_DATA(0x0086103C);

    alignas(8) uint8_t a1[0xC8];

    std::memset(edx0, 0, static_cast<size_t>(static_cast<uint32_t>(a3)) * 4u);

    uint32_t eax = 0;
    uint32_t* outSlot = reinterpret_cast<uint32_t*>(edx0);
    int index = 1;
    uint32_t bl = 0;
    do {
        hook::Regs r{};
        r.eax = static_cast<uint32_t>(a2);
        r.edx = reinterpret_cast<uintptr_t>(a1);
        r.esi = static_cast<uint32_t>(index);
        const uint32_t stack[] = {static_cast<uint32_t>(index), paramAddr};
        eax = hook::call_regs(CSF_ADDR_FN(0x00705860), &r, stack, 2);

        eax = ParticleRender_CheckImagePageAvailable(reinterpret_cast<const char*>(a1), index,
                                                     static_cast<int>(paramAddr));
        if (!(eax & 0xFF))
            break;

        eax = static_cast<uint32_t>(RwImageRead(static_cast<int>(reinterpret_cast<uintptr_t>(a1))));
        if (eax == 0)
            break;

        *outSlot = eax;                    // store the handle
        eax = static_cast<uint32_t>(a3);   // original reloads [ebp+0xc] after the store
        ++index;
        ++outSlot;
        bl = 1;
    } while (index <= a3);

    return (eax & 0xFFFFFF00u) | bl;
}

HOOK_REGS(0x00705990, ParticleRender_ReadImageBatch, 0, "gfx_particle_render");
