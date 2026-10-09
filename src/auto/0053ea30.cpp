#include <cstdint>
#include "globals.h"
#include "hook.h"

// Trivial aggregate preserves the original seven-word, by-value argument.
struct MissionInfoFormatArgument
{
    std::uint32_t words[7];
};

static_assert(sizeof(MissionInfoFormatArgument) == 28);

CSF_FUNC(void*, __cdecl, GetMissionManager, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, GetMissionEntries, 0x004FE4A0,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetMissionEntryCount, 0x0044BB80,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetMissionEntry, 0x0044BBA0,
         (void* self, void* edx, std::uint32_t index));
CSF_FUNC(void*, __cdecl, GetMissionScoringSettings, 0x00756A10, ());
CSF_FUNC(int, __cdecl, ResolveMissionEntryText, 0x00692580,
         (const char* text, int flags));
CSF_FUNC(void*, __fastcall, ConstructMissionFormatString, 0x00697100,
         (void* self, void* edx, const char* format));
CSF_FUNC(void*, __cdecl, FormatMissionEntryText, 0x00696A80,
         (void* output, MissionInfoFormatArgument format, ...));
CSF_FUNC(int, __cdecl, FreeMissionText, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, FormatMissionScore, 0x007AF9BB,
         (char* output, const char* format, ...));

CSF_GLOBAL(float, g_MissionUnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(int, g_MissionNumberFormattingMode, 0x0085E684);

static int __stdcall MenuMissionInfo_PopulateEntryScores(
    void* menu, char category, int /*unused1*/, int /*unused2*/)
{
    void* entries = GetMissionEntries(GetMissionManager(), nullptr);
    if (!entries)
        return 0;

    std::uint32_t total = 0;
    std::uint32_t index = 0;
    int row = 0;

    if (GetMissionEntryCount(entries, nullptr) != 0)
    {
        do
        {
            void* entry = GetMissionEntry(entries, nullptr, index);
            if (CSF_AT(std::int32_t, entry, 0x38) >= 0 &&
                category == CSF_AT(char, entry, 0x5C))
            {
                const bool contributesToTotal =
                    CSF_AT(std::int32_t, entry, 0x3C) == 0;

                void* settings = GetMissionScoringSettings();
                const float multiplier = category
                    ? CSF_AT(float, settings, 0x1D68)
                    : CSF_AT(float, settings, 0x1D64);

                const std::int32_t quantitySign =
                    CSF_AT(std::int32_t, entry, 0x60);
                double quantity =
                    static_cast<double>(CSF_AT(std::int32_t, entry, 0x60));
                if (quantitySign < 0)
                    quantity += static_cast<double>(g_MissionUnsignedIntegerBias);

                const std::uint32_t score = static_cast<std::uint32_t>(
                    static_cast<std::int64_t>(
                        quantity * static_cast<double>(multiplier)));

                char scoreText[16];
                FormatMissionScore(
                    scoreText, CSF_STR(0x0083090C, "%d"),
                    contributesToTotal ? static_cast<std::int32_t>(score) : 0);

                const std::int32_t status =
                    CSF_AT(std::int32_t, entry, 0x3C);
                const char* prefix = CSF_STR(0x00842DAC, "@@56/");
                if (status == 0)
                    prefix = CSF_STR(0x00842D9C, "@@58/");
                else if (status == 1)
                    prefix = CSF_STR(0x00842DA4, "@@57/");

                const char* entryText;
                if (CSF_AT(std::uint32_t, entry, 0x58) < 16)
                    entryText = reinterpret_cast<const char*>(entry) + 0x44;
                else
                    entryText = CSF_AT(const char*, entry, 0x44);

                const int resolvedText = ResolveMissionEntryText(entryText, 12);
                if (resolvedText != 0)
                {
                    const int formattingMode = g_MissionNumberFormattingMode;

                    // Stack-local string storage, not fields of a game object.
                    std::uint32_t output[7];
                    output[6] = 7;
                    output[5] = 0;
                    reinterpret_cast<std::uint16_t*>(output + 1)[0] = 0;

                    MissionInfoFormatArgument format;
                    ConstructMissionFormatString(
                        &format, nullptr, CSF_STR(0x00842D90, "{s0}{w1}"));
                    FormatMissionEntryText(
                        output, format, prefix, resolvedText, formattingMode);

                    void* displayedText = output[6] < 8
                        ? static_cast<void*>(output + 1)
                        : reinterpret_cast<void*>(output[1]);

                    using SetLabel = void(__fastcall*)(
                        void*, void*, int, int, void*, int);
                    reinterpret_cast<SetLabel>(
                        CSF_AT(void**, menu, 0)[0x294 / 4])(
                            menu, nullptr, row, 0, displayedText, -1);

                    if (output[6] >= 8 && output[1] != 0)
                        FreeMissionText(reinterpret_cast<void*>(output[1]));

                    output[6] = 7;
                    output[5] = 0;
                    reinterpret_cast<std::uint16_t*>(output + 1)[0] = 0;
                }

                using SetScore = void(__fastcall*)(
                    void*, void*, int, int, char*, int);
                reinterpret_cast<SetScore>(
                    CSF_AT(void**, menu, 0)[0x290 / 4])(
                        menu, nullptr, row, 1, scoreText, 0);

                ++row;
                if (contributesToTotal)
                    total += score;
            }

            ++index;
        }
        while (index < GetMissionEntryCount(entries, nullptr));
    }

    return static_cast<std::int32_t>(total);
}

HOOK(0x0053EA30, MenuMissionInfo_PopulateEntryScores, "auto");
