#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Global table of object slots (dword_8BB478 in the original).
CSF_GLOBAL(uint32_t, g_8BB478, 0x8BB478);

// Returns the byte size of the property block for the object at objectIndex.
// The base is 68 plus 4 * (5 or 11), plus the sizes of up to two sub-blocks.
static int __cdecl GfxObjectProps_ComputeBlockBytes(int objectIndex)
{
    // Owner object pointer stored in the slot table at (table + index + 4).
    uint8_t* slotAddress = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(g_8BB478) + static_cast<uintptr_t>(objectIndex) + 4);
    uint32_t ownerRaw = CSF_AT(uint32_t, slotAddress, 0);
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ownerRaw));

    // Properties pointer at owner+0x10.
    uint32_t propsRaw = CSF_AT(uint32_t, owner, 0x10);
    uint8_t* props = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(propsRaw));

    int blockBytes = 0;
    if (propsRaw != 0 && CSF_AT(uint32_t, props, 0) != 0) {
        uint8_t* firstBlock = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, props, 0)));
        blockBytes = CSF_AT(int, firstBlock, 0x14);
    }
    if (propsRaw != 0) {
        uint32_t secondRaw = CSF_AT(uint32_t, props, 4);
        if (secondRaw != 0) {
            uint8_t* secondBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(secondRaw));
            blockBytes += CSF_AT(int, secondBlock, 0x14);
        }
    }

    // Flag at owner+0x14 selects 11 entries when nonzero, otherwise 5.
    int flagValue = CSF_AT(int, owner, 0x14);
    int entryCount = (flagValue != 0) ? 11 : 5;

    return blockBytes + 4 * entryCount + 68;
}

HOOK(0x006B8C70, GfxObjectProps_ComputeBlockBytes, "auto");
