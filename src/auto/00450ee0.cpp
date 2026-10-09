#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, sub_44FC80, 0x0044FC80, (int a1, uint32_t* a2));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));

// Reads the currently focused entry, lets sub_44FC80 process the entry list,
// then re-finds that entry's index in the (possibly changed) list. If the entry
// is gone, the focus index becomes -1 and the scene's focus entry is cleared.
static void __fastcall CScene_RefreshFocusEntryIndex(C_008312b0* self, void* /*edx*/, int a2)
{
    uint32_t focusIndex = CSF_AT(uint32_t, self, 0x70);
    uint32_t focusedEntry = 0;
    if (focusIndex != 0xFFFFFFFFu && focusIndex < CSF_AT(uint32_t, self, 0x68))
        focusedEntry = CSF_AT(uint32_t*, self, 0x60)[focusIndex];

    sub_44FC80(a2, reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x60));

    if (focusedEntry != 0) {
        uint32_t entryCount = CSF_AT(uint32_t, self, 0x68);
        uint32_t* entries = CSF_AT(uint32_t*, self, 0x60);
        uint32_t newIndex = 0xFFFFFFFFu;
        for (uint32_t i = 0; i < entryCount; ++i) {
            if (entries[i] == focusedEntry) {
                newIndex = i;
                break;
            }
        }
        CSF_AT(uint32_t, self, 0x70) = newIndex;
        if (newIndex == 0xFFFFFFFFu)
            CurrentScene_SetFocusEntry(self, nullptr, 0);
    }
}

HOOK(0x00450EE0, CScene_RefreshFocusEntryIndex, "auto");
