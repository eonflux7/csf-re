#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, AllocateGameMemory, 0x006910F0,
         (std::uint32_t size));
CSF_FUNC(void*, __fastcall, ConstructSubtitlePanel, 0x004F31A0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetSubtitlePanelPosition, 0x004F1CE0,
         (void* self, void* edx, float x, float y));
CSF_FUNC(int, __fastcall, SetSubtitlePanelSize, 0x004F1D70,
         (void* self, void* edx, float width, float height));
CSF_FUNC(void*, __cdecl, GetGlobalContext, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, GetDisplayInterface, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, OpenSubtitleFile, 0x00698350,
         (const char* path, const char* mode, int flags));
CSF_FUNC(bool, __cdecl, ReadWideTextLine, 0x00545E00,
         (void* file, std::uint16_t* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetWideTextLength, 0x00695560,
         (std::uint16_t* text));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620,
         (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(double, __cdecl, ParseSubtitleTime, 0x007B04AF,
         (const char* text));
CSF_FUNC(void*, __fastcall, AssignWideSubtitleText, 0x00696F90,
         (void* self, void* edx, std::int16_t* text));
CSF_FUNC(std::uint32_t, __fastcall, AppendSubtitleElement, 0x005483C0,
         (void* self, void* edx, int* source));
CSF_FUNC(int, __cdecl, FreeSubtitleStringStorage, 0x00690E60,
         (void* block));
CSF_FUNC(int, __cdecl, CloseSubtitleFile, 0x00697980,
         (void* file));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_SubtitlePanelScale, 0x0082DC70);
CSF_GLOBAL(std::uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(std::uint32_t, g_ScreenHeight, 0x00899BA0);

static double ConvertUnsignedDimension(std::uint32_t value)
{
    double converted = static_cast<double>(static_cast<std::int32_t>(value));
    if (static_cast<std::int32_t>(value) < 0)
        converted += static_cast<double>(g_UnsignedIntegerBias);
    return converted;
}

static int __fastcall MenuExtras_LoadMakingOfSubtitles(
    void* self, void* /*edx*/)
{
    void* panelStorage = AllocateGameMemory(100);
    void* panel = panelStorage
        ? ConstructSubtitlePanel(panelStorage, nullptr)
        : nullptr;

    CSF_AT(void*, self, 0x48) = panel;
    SetSubtitlePanelPosition(panel, nullptr, 0.1f, 0.8f);
    CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x48), 0x24) = 1;
    CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x48), 0x34) = 0xFFFFFFFFu;
    CSF_AT(std::uint32_t, CSF_AT(void*, self, 0x48), 0x54) = 3;
    CSF_AT(std::uint8_t, CSF_AT(void*, self, 0x48), 0x58) = 1;
    CSF_AT(std::uint8_t, CSF_AT(void*, self, 0x48), 0x59) = 1;

    void* display = GetDisplayInterface(GetGlobalContext(), nullptr);
    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[6])(display, nullptr);
    const std::uint32_t width = CSF_AT(std::uint32_t, dimensions, 0);

    display = GetDisplayInterface(GetGlobalContext(), nullptr);
    dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, display, 0)[6])(display, nullptr);

    const double heightValue =
        ConvertUnsignedDimension(CSF_AT(std::uint32_t, dimensions, 4));
    const double screenHeight = ConvertUnsignedDimension(g_ScreenHeight);
    const float scaledHeight = static_cast<float>(
        (heightValue / screenHeight) * static_cast<double>(g_SubtitlePanelScale));

    const double widthValue = ConvertUnsignedDimension(width);
    const double screenWidth = ConvertUnsignedDimension(g_ScreenWidth);
    const float scaledWidth = static_cast<float>(
        (widthValue / screenWidth) * static_cast<double>(g_SubtitlePanelScale));

    SetSubtitlePanelSize(
        CSF_AT(void*, self, 0x48), nullptr, scaledWidth, scaledHeight);

    void* file = OpenSubtitleFile(
        CSF_STR(0x00843D3C, "texts/makingof.sub"),
        CSF_STR(0x0083E130, "r"), 0);
    if (!file)
        return 0;

    std::uint16_t wideLine[500];
    char narrowLine[500];

    reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, file, 0)[2])(file, nullptr, wideLine, 2);

    if (wideLine[0] == 0xFEFF)
    {
        // Timestamp followed by the original 28-byte wide-string object.
        // Leave the same unused bytes uninitialized as the original.
        alignas(4) std::uint8_t subtitleElement[32];
        CSF_AT(std::uint32_t, subtitleElement, 28) = 7;
        CSF_AT(std::uint32_t, subtitleElement, 24) = 0;
        CSF_AT(std::uint16_t, subtitleElement, 8) = 0;

        for (;;)
        {
            if (!ReadWideTextLine(file, wideLine, 500))
                break;

            const int length = GetWideTextLength(wideLine);
            ConvertWideTextToNarrow(wideLine, narrowLine, length);
            CSF_AT(float, subtitleElement, 0) =
                static_cast<float>(ParseSubtitleTime(narrowLine));

            const bool textRead = ReadWideTextLine(file, wideLine, 500);
            GetWideTextLength(wideLine);
            AssignWideSubtitleText(
                subtitleElement + 4, nullptr,
                reinterpret_cast<std::int16_t*>(wideLine));
            AppendSubtitleElement(
                static_cast<std::uint8_t*>(self) + 0x4C, nullptr,
                reinterpret_cast<int*>(subtitleElement));

            if (!textRead)
                break;
        }

        if (CSF_AT(std::uint32_t, subtitleElement, 28) >= 8)
        {
            void* block = CSF_AT(void*, subtitleElement, 8);
            if (block)
                FreeSubtitleStringStorage(block);
        }
    }

    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    return CloseSubtitleFile(file);
}

HOOK(0x00548430, MenuExtras_LoadMakingOfSubtitles, "auto");
