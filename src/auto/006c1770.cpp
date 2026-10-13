#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callback invoked by RwTexDictionaryForAllTexDictionaries for each texture dictionary.
// Original: sub_6C10B0, bound by address and used only as a value.
using TexDictIterateCallback = int(__cdecl*)(uint32_t* dictData, int userData);

// RwTexDictionaryForAllTexDictionaries: __cdecl, 2 args, pops 0.
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTexDictionaries, 0x631730,
         (TexDictIterateCallback callback, int userData));

static int __stdcall TexDict_FindNameId(uint8_t* nameText) {
    if (!nameText || *nameText == 0) {
        return 0;
    }

    // Two-dword stack record: [0] = result slot, [1] = the searched name pointer.
    uint32_t searchRecord[2];
    searchRecord[1] = (uint32_t)(uintptr_t)nameText;
    searchRecord[0] = 0;

    RwTexDictionaryForAllTexDictionaries(
        reinterpret_cast<TexDictIterateCallback>(CSF_ADDR_FN(0x006C10B0)),
        (int)(intptr_t)&searchRecord[0]);

    return (int)searchRecord[0];
}

HOOK(0x006C1770, TexDict_FindNameId, "auto");
