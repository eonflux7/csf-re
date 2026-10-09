#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0558, 0x004F4920, ());
CSF_FUNC(void, __fastcall, SetNotificationText, 0x004F49C0, (void* self, void* edx, void* a4, float a5));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, MenuMissionInfo_SetGlobalIndex, 0x00525DD0, (void* self, void* , int value));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static wchar_t* __fastcall CCharActionFsm_ShowContextNotification(
    CCharActionFsm* self, void* /*edx*/, float elapsedTime)
{
    const uint32_t owner = self->field_0008;
    uint32_t result = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (owner != result)
        return reinterpret_cast<wchar_t*>(result);

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* provider = CSF_AT(void*, manager, 0x1FC0);
    void* entry =
        reinterpret_cast<void* (__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, provider, 0)[8 / 4])(
                provider, nullptr, elapsedTime);
    void* entryString = static_cast<uint8_t*>(entry) + 0x20;

    result = static_cast<uint32_t>(CompareStringSubstring(
        entryString, nullptr, 0, CSF_AT(unsigned int, entryString, 0x14),
        reinterpret_cast<const char*>(&g_byte_82E65C), 0));
    if (!result)
        return reinterpret_cast<wchar_t*>(result);

    alignas(4) uint8_t formattedText[0x1C] = {};
    alignas(4) uint8_t formatText[0x1C] = {};
    alignas(4) uint8_t notificationText[0x1C] = {};

    CSF_AT(uint32_t, formattedText, 0x18) = 7;
    CSF_AT(uint32_t, formattedText, 0x14) = 0;
    CSF_AT(uint16_t, formattedText, 0x04) = 0;

    wchar_t* format = reinterpret_cast<wchar_t*>(
        MenuMessageBox_ResolveEntryByKey(1731342393, 8));
    ConstructWideString(formatText, nullptr, format);

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const float notificationTime = CSF_AT(float, globalState, 0x18DC);

    void* entryCharacters;
    if (CSF_AT(uint32_t, entryString, 0x18) < 0x10u)
        entryCharacters = static_cast<uint8_t*>(entryString) + 4;
    else
        entryCharacters = CSF_AT(void*, entryString, 4);

    const int formattingMode = g_NumberFormattingMode;
    const int displayText = Text_ConvertToDisplayEncoding(
        reinterpret_cast<int>(entryCharacters), 8);

    // The formatter takes an owning 28-byte string by value, followed by
    // its printf-style arguments. Its published binding exposes only the
    // output parameter.
    struct StringArgument
    {
        uint8_t bytes[0x1C];
    };
    alignas(4) StringArgument formatArgument = {};
    CSF_AT(uint32_t, &formatArgument, 0x18) = 7;
    CSF_AT(uint32_t, &formatArgument, 0x14) = 0;
    CSF_AT(uint16_t, &formatArgument, 0x04) = 0;
    AssignStringRange_488830(
        &formatArgument, nullptr, formatText, 0, 0xFFFFFFFF);

    reinterpret_cast<void* (__cdecl*)(void*, ...)>(FormatCountTextBuffer)(
        formattedText, formatArgument, displayText, formattingMode);

    wchar_t* formattedCharacters = CSF_AT(wchar_t*, formattedText, 4);
    if (CSF_AT(uint32_t, formattedText, 0x18) < 8)
        formattedCharacters = reinterpret_cast<wchar_t*>(formattedText + 4);

    ConstructWideString(notificationText, nullptr, formattedCharacters);
    void* notificationManager = reinterpret_cast<void*>(GetGlobal8B0558());
    SetNotificationText(
        notificationManager, nullptr, notificationText, notificationTime);

    if (CSF_AT(uint32_t, notificationText, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, notificationText, 4);
        if (block)
            Mem_FreeDebug(block);
    }

    void* missionInfo = reinterpret_cast<void*>(Get_008b0678());
    result = static_cast<uint32_t>(
        MenuMissionInfo_SetGlobalIndex(missionInfo, nullptr, 91));

    if (CSF_AT(uint32_t, formatText, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, formatText, 4);
        result = reinterpret_cast<uint32_t>(block);
        if (block)
            result = static_cast<uint32_t>(Mem_FreeDebug(block));
    }

    CSF_AT(uint32_t, formatText, 0x18) = 7;
    CSF_AT(uint32_t, formatText, 0x14) = 0;
    CSF_AT(uint16_t, formatText, 4) = 0;

    if (CSF_AT(uint32_t, formattedText, 0x18) >= 8)
    {
        void* block = CSF_AT(void*, formattedText, 4);
        result = reinterpret_cast<uint32_t>(block);
        if (block)
            result = static_cast<uint32_t>(Mem_FreeDebug(block));
    }

    return reinterpret_cast<wchar_t*>(result);
}

HOOK(0x00488E70, CCharActionFsm_ShowContextNotification, "auto");
