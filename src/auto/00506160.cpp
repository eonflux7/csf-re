#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of scene entry `index` inside the entry table at this->m_scenesBegin
// (entries are 88 bytes each) and sets the byte at entry offset 0x1C to 1.
static int __fastcall CSceneLoader_GetSceneEntryAndMarkActive(CSceneLoader* self, void* /*edx*/, int entryIndex)
{
    uint32_t entryTableBase = CSF_AT(uint32_t, self, 0x30);
    int entryAddress = (int)(entryTableBase + (uint32_t)entryIndex * 0x58u);

    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryAddress));
    CSF_AT(uint8_t, entry, 0x1C) = 1;

    return entryAddress;
}

HOOK(0x00506160, CSceneLoader_GetSceneEntryAndMarkActive, "auto");
