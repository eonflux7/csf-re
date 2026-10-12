#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, MenuControls_DrawText, 0x004F9CC0, (void* self, void* edx, float a2, float a3, void* String, int* a5));
CSF_FUNC(int, __fastcall, MenuControls_AppendSpawnPoint, 0x004F9ED0, (void* self, void* edx, float a2, float* a4, int arg8, int a6));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, SpawnPoints_EntryAddressForIndex, 0x005512B0, (void* self, void* , int index));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_Const20, 0x0082E0F0);
CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(float, g_LayoutInset, 0x0082FEBC);
CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_MenuHeightScale, 0x0083F250);
CSF_GLOBAL(char, g_MenuCursorMarker, 0x0083F264);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

static void* DevConsole_GetLayoutObject()
{
    return LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
}

static void* DevConsole_GetDimensions(void* object)
{
    return ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
}

static int DevConsole_TruncateCoordinate(float value)
{
    return static_cast<int>(static_cast<std::int64_t>(value));
}

// These two callees are bound with float parameters, but their callers
// pass integer-coordinate words, not integer-to-float conversions.
static float DevConsole_CoordinateArgument(int coordinate)
{
    float argument;
    std::memcpy(&argument, &coordinate, sizeof(argument));
    return argument;
}

// The formatter string is passed by value on the cdecl argument stack.
struct alignas(4) DevConsole_FormatterValue
{
    std::uint8_t bytes[0x1C];
};

static std::uint8_t __fastcall S_004f21d0_RefreshControls(
    S_004f21d0* self, void* /*edx*/)
{
    std::uint8_t result = CSF_AT(std::uint8_t, self, 0xF8);
    if (!result)
        return result;

    if (CSF_AT(std::uint8_t, self, 0x10) ||
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        void* heightObject = DevConsole_GetLayoutObject();
        void* spacingObject = DevConsole_GetLayoutObject();
        void* upperLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        void* lowerLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

        void* dimensions = DevConsole_GetDimensions(heightObject);
        float scaledHeight =
            static_cast<float>(CSF_AT(int, dimensions, 4)) * g_MenuHeightScale;
        int combinedHeight = CSF_AT(int, upperLayout, 0x2238) +
                             CSF_AT(int, lowerLayout, 0x2240);
        int initialY = DevConsole_TruncateCoordinate(
            scaledHeight * static_cast<float>(combinedHeight));

        dimensions = DevConsole_GetDimensions(spacingObject);
        initialY -= DevConsole_TruncateCoordinate(
            static_cast<float>(CSF_AT(int, dimensions, 4)) *
            g_MenuHeightScale * g_Const20);
        float currentY = DevConsole_CoordinateArgument(initialY);

        int displayedCount = 0;
        int entryIndex = 0;

        heightObject = DevConsole_GetLayoutObject();
        spacingObject = DevConsole_GetLayoutObject();
        void* layout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

        dimensions = DevConsole_GetDimensions(heightObject);
        int availableHeight = DevConsole_TruncateCoordinate(
            static_cast<float>(CSF_AT(int, dimensions, 4)) *
            g_MenuHeightScale * static_cast<float>(CSF_AT(int, layout, 0x2240)));
        dimensions = DevConsole_GetDimensions(spacingObject);
        int rowHeight = DevConsole_TruncateCoordinate(
            static_cast<float>(CSF_AT(int, dimensions, 4)) *
            g_MenuHeightScale * g_LayoutInset);

        if (-2 - availableHeight / rowHeight > 0)
        {
            do
            {
                if (GetGlobal8B0688())
                {
                    void* spawnPoints = reinterpret_cast<void*>(GetGlobal8B0688());
                    int entry = SpawnPoints_EntryAddressForIndex(
                        spawnPoints, nullptr, entryIndex);
                    if (entry)
                    {
                        heightObject = DevConsole_GetLayoutObject();
                        spacingObject = DevConsole_GetLayoutObject();
                        void* widthObject = DevConsole_GetLayoutObject();
                        void* heightLayout =
                            reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                        void* widthLayout =
                            reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

                        void* widthDimensions = DevConsole_GetDimensions(widthObject);
                        dimensions = DevConsole_GetDimensions(heightObject);
                        availableHeight = DevConsole_TruncateCoordinate(
                            static_cast<float>(CSF_AT(int, dimensions, 4)) *
                            g_MenuHeightScale *
                            static_cast<float>(CSF_AT(int, heightLayout, 0x2240)));

                        dimensions = DevConsole_GetDimensions(spacingObject);
                        rowHeight = DevConsole_TruncateCoordinate(
                            static_cast<float>(CSF_AT(int, dimensions, 4)) *
                            g_MenuHeightScale * g_LayoutInset);

                        int remainingRows =
                            -2 - availableHeight / rowHeight - displayedCount;
                        int x = DevConsole_TruncateCoordinate(
                            static_cast<float>(CSF_AT(int, widthDimensions, 0)) *
                            g_MenuWidthScale *
                            static_cast<float>(CSF_AT(int, widthLayout, 0x2234)));

                        int previousCount = displayedCount;
                        displayedCount = previousCount +
                            MenuControls_AppendSpawnPoint(
                                self, nullptr, DevConsole_CoordinateArgument(x),
                                &currentY, entry, remainingRows);
                    }
                }

                ++entryIndex;
                heightObject = DevConsole_GetLayoutObject();
                spacingObject = DevConsole_GetLayoutObject();
                layout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

                dimensions = DevConsole_GetDimensions(heightObject);
                availableHeight = DevConsole_TruncateCoordinate(
                    static_cast<float>(CSF_AT(int, dimensions, 4)) *
                    g_MenuHeightScale *
                    static_cast<float>(CSF_AT(int, layout, 0x2240)));
                dimensions = DevConsole_GetDimensions(spacingObject);
                rowHeight = DevConsole_TruncateCoordinate(
                    static_cast<float>(CSF_AT(int, dimensions, 4)) *
                    g_MenuHeightScale * g_LayoutInset);
            }
            while (displayedCount < -2 - availableHeight / rowHeight);
        }
    }

    result = CSF_AT(std::uint8_t, self, 0x10);
    if (!result)
        return result;

    // The string spans the adjacent -0x38 and -0x34 frame regions.
    alignas(8) std::uint8_t textBytes[0x38] = {};
    CSF_AT(std::uint32_t, textBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, textBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, textBytes, 4) = 0;

    std::uint32_t messageKind = CSF_AT(std::uint32_t, self, 0x100);
    int messageKey = 1129072689;
    if (messageKind == 1)
        messageKey = 1129072690;
    else if (messageKind == 2)
        messageKey = 1129072688;

    int message = MenuMessageBox_ResolveEntryByKey(messageKey, 2);
    const char* suffix = &g_MenuCursorMarker;
    if (!CSF_AT(std::uint8_t, self, 0xE0))
        suffix = reinterpret_cast<const char*>(&g_byte_82E65C);

    int formattingMode = g_NumberFormattingMode;
    void* inputText = reinterpret_cast<std::uint8_t*>(self) + 0x18;

    DevConsole_FormatterValue formatter = {};
    FormatterSetPattern(
        formatter.bytes, nullptr, CSF_STR(0x0083F254, "{w0}{w1}{s2}"));

    // The packet's fixed-argument binding omits the by-value formatter
    // and its following formatting arguments.
    using FormatWithArguments = void*(__cdecl*)(void*, ...);
    void* formatted = reinterpret_cast<FormatWithArguments>(FormatCountTextBuffer)(
        textBytes, formatter, message, inputText, suffix, formattingMode);
    AssignStringRange_488830(textBytes, nullptr, formatted, 0, 0xFFFFFFFF);

    void* heightObject = DevConsole_GetLayoutObject();
    void* widthObject = DevConsole_GetLayoutObject();

    void* text = CSF_AT(std::uint32_t, textBytes, 0x18) < 8
        ? static_cast<void*>(textBytes + 4)
        : CSF_AT(void*, textBytes, 4);

    void* upperLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* lowerLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* widthLayout = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    void* widthDimensions = DevConsole_GetDimensions(widthObject);
    void* heightDimensions = DevConsole_GetDimensions(heightObject);
    float scaledHeight =
        static_cast<float>(CSF_AT(int, heightDimensions, 4)) * g_MenuHeightScale;
    int combinedHeight = CSF_AT(int, upperLayout, 0x2238) +
                         CSF_AT(int, lowerLayout, 0x2240);
    int x = DevConsole_TruncateCoordinate(
        static_cast<float>(CSF_AT(int, widthDimensions, 0)) *
        g_MenuWidthScale * static_cast<float>(CSF_AT(int, widthLayout, 0x2234)));
    int y = DevConsole_TruncateCoordinate(
        scaledHeight * static_cast<float>(combinedHeight));

    result = static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(
        MenuControls_DrawText(
            self, nullptr, DevConsole_CoordinateArgument(x),
            DevConsole_CoordinateArgument(y), text, nullptr)));

    if (CSF_AT(std::uint32_t, textBytes, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, textBytes, 4);
        result = static_cast<std::uint8_t>(reinterpret_cast<std::uintptr_t>(block));
        if (block)
            return static_cast<std::uint8_t>(Mem_FreeDebug(block));
    }
    return result;
}

HOOK(0x004FA2D0, S_004f21d0_RefreshControls, "auto");
