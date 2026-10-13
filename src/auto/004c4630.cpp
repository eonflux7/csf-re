#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(bool, __fastcall, GoToCover_HandleEntry, 0x004C4500, (void* self, void* edx, int arg0, int a3));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_LevelScaleFactor, 0x0082E048);

// Walks the counted list of entry pointers (list[0] = array, list[2] = count).
// Entries with a non-negative flag at +0x1C and whose +0x30 position plus the
// scale factor lies beyond the current scene's reference value are handed to
// GoToCover_HandleEntry.
static void __fastcall GoToCover_ProcessEntryList(S_004bd310* self, void* /*edx*/, uint32_t* entryList)
{
    uint32_t entryCount = entryList[2];
    for (uint32_t i = 0; i < entryCount; ++i)
    {
        uint8_t* entry = reinterpret_cast<uint8_t*>(reinterpret_cast<uint32_t*>(entryList[0])[i]);

        if (CSF_AT(int, entry, 0x1C) >= 0 &&
            CSF_AT(float, entry, 0x30) + g_LevelScaleFactor >
                CSF_AT(float, CSF_AT(void*, g_pCurrentScene, 0xB0), 8))
        {
            GoToCover_HandleEntry(self, nullptr, CSF_AT(int, entry, 8), static_cast<int>(reinterpret_cast<uintptr_t>(entry + 0x14)));
        }
    }
}

HOOK(0x004C4630, GoToCover_ProcessEntryList, "auto");
