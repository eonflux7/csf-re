#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Sets the active spawn index on the spawn-point owner. When it changes, marks the
// owner dirty (+0x148C), stores the index (+0x13D0), and mirrors the low 16 bits into
// the linked child's word at +0x446 (child = owner->+0xC8C->+0x18 - 0x6B0), unless that
// value already matches.
static void __fastcall MpSpawn_SetCurrentSpawnIndex(void* self, void* /*edx*/, int spawnIndex)
{
    if (spawnIndex == CSF_AT(int, self, 0x13D0))
        return;

    CSF_AT(uint8_t, self, 0x148C) = 1;
    CSF_AT(int, self, 0x13D0) = spawnIndex;

    const uint32_t ownerRaw = CSF_AT(uint32_t, self, 0xC8C);
    if (ownerRaw == 0)
        return;

    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ownerRaw));
    const uint32_t childLinkRaw = CSF_AT(uint32_t, owner, 0x18);
    if (childLinkRaw == 0)
        return;

    if (childLinkRaw == 0x6B0)
        return;

    uint8_t* child = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(childLinkRaw - 0x6B0));
    const uint16_t newWord = static_cast<uint16_t>(spawnIndex);
    if (newWord != CSF_AT(uint16_t, child, 0x446))
        CSF_AT(uint16_t, child, 0x446) = newWord;
}

HOOK(0x5514E0, MpSpawn_SetCurrentSpawnIndex, "auto");
