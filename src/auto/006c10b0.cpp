#include <cstdint>
#include "globals.h"
#include "hook.h"

// Looks up a named texture in the RenderWare texture dictionary.
CSF_FUNC(char*, __cdecl, RwTexDictionaryFindNamedTexture, 0x00631950, (int dict, const char* name));

static int __cdecl GfxTexDict_FindAndStoreNamedTexture(int dictionary, uint32_t* outTextureSlot)
{
    char* foundTexture = RwTexDictionaryFindNamedTexture(dictionary, reinterpret_cast<const char*>(outTextureSlot[1]));
    if (foundTexture) {
        *reinterpret_cast<char**>(outTextureSlot) = foundTexture;
    }
    return dictionary;
}

HOOK(0x6C10B0, GfxTexDict_FindAndStoreNamedTexture, "auto");
