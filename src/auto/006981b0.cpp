#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);
CSF_FUNC(char, __cdecl, SceneLoader_ResetArchiveSelection, 0x006980D0, (char* String1));

static void __cdecl CoreFiles_RestorePreviousLog()
{
    const uint32_t begin = CSF_AT(uint32_t, g_pOptions, 0x54);
    if (!begin)
        return;

    const int32_t count =
        static_cast<int32_t>(CSF_AT(uint32_t, g_pOptions, 0x58) - begin) >> 5;
    if (!count)
        return;

    const uint32_t offset = 32 * (static_cast<uint32_t>(count) + 0x7FFFFFF);
    auto* entry = reinterpret_cast<uint8_t*>(begin + offset);

    char* selection;
    if (CSF_AT(uint32_t, entry, 0x18) < 0x10)
        selection = reinterpret_cast<char*>(entry + 4);
    else
        selection = CSF_AT(char*, entry, 4);

    SceneLoader_ResetArchiveSelection(selection);
}

HOOK(0x006981B0, CoreFiles_RestorePreviousLog, "auto");
