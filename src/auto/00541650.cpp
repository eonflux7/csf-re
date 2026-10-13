#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, ProfilesSavegames_ReleaseSlotBuffer, 0x005129A0, (int firstSlot, int endSlot));
CSF_FUNC(std::uint32_t*, __fastcall, S_005139c0_AppendElement200, 0x00513A80, (S_005139c0* self, void* , int a3));
CSF_FUNC(int, __fastcall, CProfileManager_GetFirstFlaggedEntryValue, 0x00515C20, (uint32_t* self, void*));
CSF_FUNC(int*, __fastcall, ProfileSavegames_GetScenarioCollection, 0x00515CE0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_IsSameEntry, 0x00541030, (void* self, void* , void* other));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int* __cdecl MenuSaveGame_RebuildListContainer(S_005139c0* container)
{
    if (container->field_0004)
    {
        ProfilesSavegames_ReleaseSlotBuffer(
            static_cast<int>(container->field_0004),
            static_cast<int>(container->field_0008));
        if (container->field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(container->field_0004));
    }

    container->field_0004 = 0;
    container->field_0008 = 0;
    container->field_000c = 0;

    void* entryProvider = reinterpret_cast<void*>(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x4));
    auto* profileManager = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x8));

    int* scenarios = ProfileSavegames_GetScenarioCollection(profileManager, nullptr);
    const std::uint32_t scenarioBegin = CSF_AT(std::uint32_t, scenarios, 0x4);
    if (!scenarioBegin)
        return scenarios;

    const std::int32_t scenarioCount = static_cast<std::int32_t>(
        CSF_AT(std::uint32_t, scenarios, 0x8) - scenarioBegin) / 80;
    if (!scenarioCount)
        return nullptr;

    void* currentProfile =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, profileManager, 0)[0x24 / 4])(
                profileManager, nullptr);

    // The low byte controls the branch, but the full result is returned.
    int* profileStatus =
        reinterpret_cast<int* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, profileManager, 0)[0x4c / 4])(
                profileManager, nullptr, currentProfile);
    if (static_cast<std::uint8_t>(
            reinterpret_cast<std::uintptr_t>(profileStatus)) != 0)
        return profileStatus;

    void* entries =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, entryProvider, 0)[0x18 / 4])(
                entryProvider, nullptr, 0);

    const std::uint32_t entriesBegin = CSF_AT(std::uint32_t, entries, 0x4);
    std::uint32_t remaining = 0;
    if (entriesBegin)
    {
        remaining = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, entries, 0x8) - entriesBegin) / 200);
    }
    if (!remaining)
        return nullptr;

    alignas(8) std::uint8_t comparisonBytes[0x24];
    std::uint32_t entryOffset = 0;

    do
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, entries, 0x4) + entryOffset);

        bool append = CSF_AT(std::uint8_t, entry, 0xc0) != 0;
        if (!append)
        {
            const int selectedValue =
                CProfileManager_GetFirstFlaggedEntryValue(profileManager, nullptr);

            void* profile =
                reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, profileManager, 0)[0x24 / 4])(
                        profileManager, nullptr);
            auto* comparisonSource = static_cast<std::uint8_t*>(profile) + 0x1c;

            CSF_AT(std::uint32_t, comparisonBytes, 0x00) =
                CSF_AT(std::uint32_t, comparisonSource, 0x00);
            CSF_AT(std::uint32_t, comparisonBytes, 0x04) =
                CSF_AT(std::uint32_t, comparisonSource, 0x04);
            CSF_AT(std::uint32_t, comparisonBytes, 0x08) =
                CSF_AT(std::uint32_t, comparisonSource, 0x08);
            CSF_AT(std::uint32_t, comparisonBytes, 0x0c) =
                CSF_AT(std::uint32_t, comparisonSource, 0x0c);
            CSF_AT(std::uint32_t, comparisonBytes, 0x10) =
                CSF_AT(std::uint32_t, comparisonSource, 0x10);
            CSF_AT(std::uint32_t, comparisonBytes, 0x14) =
                CSF_AT(std::uint32_t, comparisonSource, 0x14);

            if (CSF_AT(int, entry, 0x34) == selectedValue)
            {
                append = static_cast<std::uint8_t>(
                    CMenuMpScoreBar_IsSameEntry(
                        entry + 0x38, nullptr, comparisonBytes)) != 0;
            }
        }

        if (append)
        {
            S_005139c0_AppendElement200(
                container, nullptr, reinterpret_cast<int>(entry));
        }

        entryOffset += 200;
        --remaining;
    } while (remaining);

    return nullptr;
}

HOOK(0x00541650, MenuSaveGame_RebuildListContainer, "auto");
