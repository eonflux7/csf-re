#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

static int __fastcall CGuiTacticalMap_GetSpriteForEntry(
    CGuiTacticalMap* self, void* /*edx*/, int entryAddress)
{
    if (!entryAddress)
        return 0;

    void* entry = reinterpret_cast<void*>(entryAddress);
    const int stateCode = CSF_AT(int, entry, 0x50);

    if (stateCode == 3)
    {
        const int medicSprite = static_cast<int>(self->m_medicSprite);
        if (medicSprite)
            return medicSprite;
    }

    const int category = CSF_AT(int, entry, 0x78);
    if (category == -1)
        return CSF_AT(int, reinterpret_cast<void*>(self->m_sprites), 0x54);

    if (category == 1)
    {
        if (static_cast<std::uint8_t>(GameConfig_IsStateCodeAllowed(stateCode)))
            return CSF_AT(int, reinterpret_cast<void*>(self->m_sprites), 0x04);

        if (static_cast<std::uint8_t>(GameConfig_IsResolvedEntryActive(stateCode)))
            return CSF_AT(int, reinterpret_cast<void*>(self->m_sprites), 0x14);
    }
    else if (category == 0)
    {
        if (static_cast<std::uint8_t>(GameConfig_IsStateCodeAllowed(stateCode)))
            return CSF_AT(int, reinterpret_cast<void*>(self->m_sprites), 0x24);

        if (static_cast<std::uint8_t>(GameConfig_IsResolvedEntryActive(stateCode)))
            return CSF_AT(int, reinterpret_cast<void*>(self->m_sprites), 0x34);
    }

    return 0;
}

HOOK(0x00437820, CGuiTacticalMap_GetSpriteForEntry, "auto");
