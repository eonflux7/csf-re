#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));

CSF_GLOBAL(int, g_TexDictEnumResetFlag, 0x008BB49C);

// Enumerates every texture of the current RenderWare texture dictionary.
// Resets the global flag before enumeration; returns the enumeration result,
// or 0 when no dictionary is current.
static int __cdecl TexDict_EnumerateCurrentTextures()
{
    int currentDictionary = RwTexDictionaryGetCurrent();
    if (currentDictionary)
    {
        g_TexDictEnumResetFlag = 0;
        return RwTexDictionaryForAllTextures(
            currentDictionary,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C10E0)),
            0);
    }
    return currentDictionary;
}

HOOK(0x006C1740, TexDict_EnumerateCurrentTextures, "auto");
