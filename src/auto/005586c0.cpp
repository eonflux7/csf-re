#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall MpRules_FindLoadoutEntries(
    std::uint32_t* self, void* /*edx*/, int spawnPointId, int loadoutId)
{
    const std::uint32_t initialBegin = CSF_AT(std::uint32_t, self, 0x1DC);
    std::uint32_t result = 0;
    std::uint32_t entryIndex = 0;
    std::uint32_t entryOffset = 0;

    do
    {
        if (!initialBegin)
            break;

        const std::int32_t entryCount =
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x1E0) - initialBegin) / 20;
        if (entryIndex >= static_cast<std::uint32_t>(entryCount))
            break;

        void* entry = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x1DC) + entryOffset);

        if (CSF_AT(std::int32_t, entry, 0x0) == spawnPointId)
        {
            std::uint32_t loadoutIndex = 0;
            std::uint32_t loadoutOffset = 0;

            do
            {
                const std::uint32_t loadoutBegin =
                    CSF_AT(std::uint32_t, entry, 0x8);
                if (!loadoutBegin)
                    break;

                const std::int32_t loadoutCount =
                    static_cast<std::int32_t>(
                        CSF_AT(std::uint32_t, entry, 0xC) - loadoutBegin) / 20;
                if (loadoutIndex >= static_cast<std::uint32_t>(loadoutCount))
                    break;

                void* loadout = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, entry, 0x8) + loadoutOffset);
                if (CSF_AT(std::int32_t, loadout, 0x0) == loadoutId)
                {
                    result = CSF_AT(std::uint32_t, entry, 0x8)
                           + loadoutOffset + 4;
                }

                ++loadoutIndex;
                loadoutOffset += 20;
            }
            while (!result);
        }

        ++entryIndex;
        entryOffset += 20;
    }
    while (!result);

    return static_cast<std::int32_t>(result);
}

HOOK(0x005586C0, MpRules_FindLoadoutEntries, "auto");
