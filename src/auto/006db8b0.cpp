#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwImageWrite, 0x006257E0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwImageDestroy, 0x00625F10, (uint32_t* a1));
CSF_FUNC(char, __cdecl, GetOptionsByteAt2, 0x006972F0, ());
CSF_FUNC(char, __cdecl, SetOptionsByteAt40, 0x00697330, (char value));
CSF_FUNC(char, __cdecl, GetOptionsByte40, 0x00697350, ());
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(uint32_t, g_ScreenshotNamePart0, 0x008612DC);
CSF_GLOBAL(uint32_t, g_ScreenshotNamePart1, 0x008612E0);
CSF_GLOBAL(uint32_t, g_ScreenshotNamePart2, 0x008612E4);
CSF_GLOBAL(uint32_t, g_ScreenshotNamePart3, 0x008612E8);
CSF_GLOBAL(uint16_t, g_ScreenshotNamePart4, 0x008612EC);

static bool __fastcall CRenderEngine_SaveScreenshot(
    CRenderEngine* self, void* /*edx*/)
{
    const int image =
        reinterpret_cast<int (__fastcall*)(CRenderEngine*, void*)>(
            CSF_AT(void**, self, 0)[0x7C / 4])(self, nullptr);
    if (!image)
        return false;

    alignas(8) char filename[0x3C] = {};
    std::memcpy(filename, &g_ScreenshotNamePart0, sizeof(uint32_t));
    std::memcpy(filename + 4, &g_ScreenshotNamePart1, sizeof(uint32_t));
    std::memcpy(filename + 8, &g_ScreenshotNamePart2, sizeof(uint32_t));
    std::memcpy(filename + 12, &g_ScreenshotNamePart3, sizeof(uint32_t));
    std::memcpy(filename + 16, &g_ScreenshotNamePart4, sizeof(uint16_t));

    const char savedOption40 = GetOptionsByte40();
    const char savedFileOption = GetOptionsByteAt2();
    SetOptionsByteAt40(1);
    CoreFiles_SetFileOption(0);

    for (int index = 0; index <= 999; ++index)
    {
        _sprintf(filename, CSF_STR(0x008612C8, "screenshot%03d.bmp"), index);
        if (static_cast<uint8_t>(CoreFiles_CheckPageAvailability(filename, 0)) == 0)
            break;
    }

    const bool written = RwImageWrite(image, reinterpret_cast<int>(filename)) != 0;
    CoreFiles_SetFileOption(savedFileOption);
    SetOptionsByteAt40(savedOption40);
    RwImageDestroy(reinterpret_cast<uint32_t*>(image));
    return written;
}

HOOK(0x006DB8B0, CRenderEngine_SaveScreenshot, "auto");
