#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(void*, __fastcall, ProcessEntryPair, 0x00472B60, (void* self, void* edx, int a2));

// Resolves two packed handles (table index in the low 23 bits, bit 23 selects the
// scene's secondary table) into element addresses, then forwards them to ProcessEntryPair.
static int __fastcall ResolveEntryPairAndInvoke(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    uint32_t entryB = a4[1];
    uint32_t keyA = CSF_AT(uint32_t, reinterpret_cast<void*>(a4[0]), 8);
    uint32_t table = CSF_AT(uint32_t, self, 0x20);
    void* scene = g_pCurrentScene;

    // First handle: element address = table[+8] + 12 * index.
    uint32_t tableForA = table;
    if ((keyA & 0x800000u) != 0)
    {
        tableForA = CSF_AT(uint32_t, CSF_AT(void*, scene, 0xBC), 0x38);
        keyA &= ~0x800000u;
    }
    uint32_t elementA = CSF_AT(uint32_t, reinterpret_cast<void*>(tableForA), 8) + 12u * keyA;

    // Second handle: only resolved when it is not -1.
    uint32_t keyB = CSF_AT(uint32_t, reinterpret_cast<void*>(entryB), 8);
    if (keyB != 0xFFFFFFFFu)
    {
        if ((keyB & 0x800000u) != 0)
        {
            table = CSF_AT(uint32_t, CSF_AT(void*, scene, 0xBC), 0x38);
            keyB &= ~0x800000u;
        }
        entryB = CSF_AT(uint32_t, reinterpret_cast<void*>(table), 8) + 12u * keyB;
    }

    void* result = ProcessEntryPair(reinterpret_cast<void*>(elementA), nullptr, static_cast<int>(entryB));
    return static_cast<int>(reinterpret_cast<uintptr_t>(result));
}

HOOK(0x4649D0, ResolveEntryPairAndInvoke, "auto");
