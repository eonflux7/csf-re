#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));

// Resolves a 12-byte table entry (selected by a descriptor index, optionally
// relative to the current scene's owner), calls the lookup, and stores the
// result as a float into the object referenced by a2+4.
static int __fastcall ResolveEntryAndStoreFloat(void* self, void* /*edx*/, void* a2, void* /*a3*/, void* a4)
{
    // Descriptor word: *(*(a4) + 8)
    uint32_t innerPtr = CSF_AT(uint32_t, a4, 0);
    uint32_t entryIndex = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(innerPtr)), 8);

    // Default owner: self->field at +0x20
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20)));

    if ((entryIndex & 0x800000u) != 0)
    {
        // Flagged entries are resolved against the current scene's child object.
        uint32_t sceneChild = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        owner = reinterpret_cast<void*>(static_cast<uintptr_t>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(sceneChild)), 0x38)));
        entryIndex &= ~0x800000u;
    }

    // Target object pointer read before the call, as in the original.
    uint32_t targetObject = CSF_AT(uint32_t, a2, 4);

    // Table base at owner+8; entry is 12 bytes, value pointer at +4.
    uint32_t tableBase = CSF_AT(uint32_t, owner, 8);
    uint32_t entryValue = *reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(tableBase) + static_cast<uintptr_t>(entryIndex) * 12u + 4u);

    int result = ResolveEntryLookupValue(reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue)), a2);

    // Signed int -> float; negative values get the unsigned bias added.
    float storedValue = static_cast<float>(result);
    if (result < 0)
        storedValue += g_UnsignedIntegerBias;

    *reinterpret_cast<float*>(static_cast<uintptr_t>(targetObject) + 4) = storedValue;

    return result;
}

HOOK(0x004647B0, ResolveEntryAndStoreFloat, "auto");
