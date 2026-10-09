#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(unsigned int, __fastcall, SceneTableEntry_Apply, 0x00473D60, (void* self, void* edx, unsigned int a2));

// Looks up a table entry by index (bit 0x800000 selects the current scene's table
// instead of the owner's table), converts a float parameter to an integer and
// applies that entry with the parameter.
static unsigned int __fastcall SceneTable_DispatchIndexedEntry(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    // Request header: *a4 -> { ..., +8: entry index with flag bit }
    void* requestHeader = reinterpret_cast<void*>(a4[0]);
    uint32_t entryIndex = CSF_AT(uint32_t, requestHeader, 8);

    // Owner table pointer, default from this+0x20
    uint32_t tableOwner = CSF_AT(uint32_t, self, 0x20);

    if ((entryIndex & 0x800000u) != 0)
    {
        // Flag set: use the current scene's table owner (scene+0xBC -> +0x38)
        uint32_t sceneObject = CSF_AT(uint32_t, g_pCurrentScene, 0xBC);
        tableOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(sceneObject), 0x38);
        entryIndex &= ~0x800000u;
    }

    // Float parameter: a4[1] -> { ..., +4: holder -> +4: float }
    void* paramBlock = reinterpret_cast<void*>(a4[1]);
    void* paramHolder = reinterpret_cast<void*>(CSF_AT(uint32_t, paramBlock, 4));
    float timeValue = CSF_AT(float, paramHolder, 4);

    uint32_t timeArg = static_cast<uint32_t>(static_cast<int64_t>(timeValue));

    // Table: owner+8 -> base; entry is 12 bytes, pointer at +4 of each entry
    uint32_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(tableOwner), 8);
    uint8_t* entryAddress = reinterpret_cast<uint8_t*>(tableBase) + static_cast<size_t>(entryIndex) * 12 + 4;
    uint32_t entryObject = *reinterpret_cast<uint32_t*>(entryAddress);

    return SceneTableEntry_Apply(reinterpret_cast<void*>(entryObject), nullptr, timeArg);
}

HOOK(0x004644B0, SceneTable_DispatchIndexedEntry, "auto");
