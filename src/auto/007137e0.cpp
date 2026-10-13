#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::int32_t, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));

// Import slots (IAT entries) used by the original through indirect calls.
using GetKeyboardLayoutFn = void* (__stdcall*)(std::uint32_t threadId);
using GetLocaleInfoAFn = int(__stdcall*)(std::uint32_t locale, std::uint32_t lcType, char* lcData, int cchData);
using MultiByteToWideCharFn = int(__stdcall*)(std::uint32_t codePage, std::uint32_t flags, const char* multiByteStr,
                                              int cbMultiByte, wchar_t* wideCharStr, int cchWideChar);

CSF_GLOBAL(GetKeyboardLayoutFn, g_GetKeyboardLayout, 0x0082D278);
CSF_GLOBAL(GetLocaleInfoAFn, g_GetLocaleInfoA, 0x0082D224);
CSF_GLOBAL(MultiByteToWideCharFn, g_MultiByteToWideChar, 0x0082D0BC);

// Reads the active keyboard layout's locale, converts the locale's code-page
// string held in this object into wide chars, and feeds each wide char to the
// handler object's slot 4 method. Returns the low word of the last handler result.
static std::int16_t __fastcall CInputDirectInput_ForwardLocaleCharsToHandler(CInputDirectInput* self, void* /*edx*/, int a2)
{
    wchar_t WideCharStr[64];
    char LCData[12];

    std::memset(WideCharStr, 0, sizeof(WideCharStr));

    void* keyboardLayoutHandle = g_GetKeyboardLayout(0);
    std::uint16_t keyboardLayout = static_cast<std::uint16_t>(reinterpret_cast<std::uintptr_t>(keyboardLayoutHandle));

    int localeInfo = g_GetLocaleInfoA(keyboardLayout, 0x1004u, LCData, 10);
    if (localeInfo)
    {
        localeInfo = ParseThumbnailIndex(LCData);
        if (localeInfo)
        {
            g_MultiByteToWideChar(static_cast<std::uint32_t>(localeInfo), 0,
                                  reinterpret_cast<const char*>(reinterpret_cast<std::uint8_t*>(self) + 0xC50),
                                  -1, WideCharStr, 64);

            void* handler = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a2));
            for (int i = 0; i < 64; ++i)
            {
                localeInfo = WideCharStr[i];
                if (!static_cast<std::uint16_t>(localeInfo))
                    break;

                auto callback = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(CSF_AT(void**, handler, 0)[4]);
                int callbackResult = callback(handler, nullptr, localeInfo);
                localeInfo = static_cast<std::uint16_t>(callbackResult);
            }
        }
    }
    return static_cast<std::int16_t>(localeInfo);
}

HOOK(0x007137E0, CInputDirectInput_ForwardLocaleCharsToHandler, "auto");
