#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(char*, __cdecl, RwTexDictionaryFindNamedTexture, 0x00631950, (int dict, const char* name));

// Returns the texture named by `name` from the current texture dictionary,
// or 0 when the name pointer is null or the string is empty.
static int __cdecl GfxAnimInterpolator_FindCurrentTextureByName(const char* name) {
    if (!name || *name == 0) {
        return 0;
    }
    int currentDictionary = RwTexDictionaryGetCurrent();
    char* texture = RwTexDictionaryFindNamedTexture(currentDictionary, name);
    return reinterpret_cast<int>(texture);
}

HOOK(0x707D30, GfxAnimInterpolator_FindCurrentTextureByName, "auto");
