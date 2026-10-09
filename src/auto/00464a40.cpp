#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AsPtr(uint32_t address) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

// __thiscall int (this, a2, a3, a4), ret 12: a2/a3 unused, a4 is a pointer to a
// struct whose first dword points at a record (+8 = flags/handle), second dword
// points at a vector-like struct whose +4 float is added to a node's +4 float.
static int __fastcall AddYOffsetToBoundNode(void* self, void* /*edx*/,
                                            int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint32_t handleWord = CSF_AT(uint32_t, AsPtr(a4[0]), 8);

    uint32_t tableOwner = CSF_AT(uint32_t, self, 0x20);
    if ((0x800000u & handleWord) != 0) {
        uint32_t sceneSubObject = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        tableOwner = CSF_AT(uint32_t, AsPtr(sceneSubObject), 0x38);
        handleWord &= ~0x800000u;
    }

    uint32_t nodeTable = CSF_AT(uint32_t, AsPtr(tableOwner), 8);
    uint32_t entryAddress = nodeTable + handleWord * 12u;
    uint32_t nodeAddress = CSF_AT(uint32_t, AsPtr(entryAddress), 4);

    uint32_t offsetVector = a4[1];
    uint32_t offsetYBlock = CSF_AT(uint32_t, AsPtr(offsetVector), 4);
    float sourceY = CSF_AT(float, AsPtr(offsetYBlock), 4);

    CSF_AT(float, AsPtr(nodeAddress), 4) =
        sourceY + CSF_AT(float, AsPtr(nodeAddress), 4);

    return static_cast<int>(nodeAddress);
}

HOOK(0x00464A40, AddYOffsetToBoundNode, "auto");
