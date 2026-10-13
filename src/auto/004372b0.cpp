#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiTacticalMap_ApplySpriteVariant, 0x00437230, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

static int __fastcall CGuiTacticalMap_SelectSpriteVariant(
    void* self, void* /*edx*/, int entryAddress)
{
    void* entry = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(entryAddress)));

    const std::uint32_t category = CSF_AT(std::uint32_t, entry, 0x24) + 1;
    const int stateCode = CSF_AT(int, entry, 0x20);

    switch (category)
    {
    case 0:
        return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 21);

    case 1:
        if (static_cast<std::uint8_t>(
                GameConfig_IsStateCodeAllowed(static_cast<std::uint32_t>(stateCode))))
        {
            return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 9);
        }
        if (static_cast<std::uint8_t>(GameConfig_IsResolvedEntryActive(stateCode)))
        {
            return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 13);
        }
        return 0;

    case 2:
        if (static_cast<std::uint8_t>(
                GameConfig_IsStateCodeAllowed(static_cast<std::uint32_t>(stateCode))))
        {
            return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 1);
        }
        if (static_cast<std::uint8_t>(GameConfig_IsResolvedEntryActive(stateCode)))
        {
            return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 5);
        }
        return 0;

    case 3:
        if (static_cast<std::uint8_t>(
                GameConfig_IsStateCodeAllowed(static_cast<std::uint32_t>(stateCode))))
        {
            return CGuiTacticalMap_ApplySpriteVariant(self, nullptr, entryAddress, 9);
        }
        return 0;

    default:
        return 0;
    }
}

HOOK(0x004372B0, CGuiTacticalMap_SelectSpriteVariant, "auto");
