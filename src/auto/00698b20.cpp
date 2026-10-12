#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(char, __cdecl, CoreFiles_IsListedPage, 0x006978C0, (int a1, int a2));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, OpenSceneFile, 0x00698350, (const char* path, const char* mode, int flags));

static int __cdecl CoreFiles_CheckPageAvailability(const char* path, char bypassList)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, g_pOptions, 0x54);
    if (begin &&
        (static_cast<std::int32_t>(
             CSF_AT(std::uint32_t, g_pOptions, 0x58) - begin) >> 5) &&
        !bypassList)
    {
        if (CoreFiles_IsListedPage(
                reinterpret_cast<int>(path),
                reinterpret_cast<int>(
                    reinterpret_cast<std::uint8_t*>(g_pOptions) + 0x50)))
            return 1;

        if (!CSF_AT(std::uint8_t, g_pOptions, 0x40))
            return 0;
    }

    void* file = OpenSceneFile(path, CSF_STR(0x0083E130, "r"), 1);
    if (file)
        CloseSceneFile(file);
    return file != nullptr;
}

HOOK(0x00698B20, CoreFiles_CheckPageAvailability, "auto");
