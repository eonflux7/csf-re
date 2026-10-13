#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, RwTexDictionaryFindNamedTexture, 0x00631950, (int dict, const char* name));
CSF_FUNC(int, __cdecl, RwTexDictionaryAddTexture, 0x006318E0, (void* dictionary, void* texture));

// Adds the texture (a1) to the dictionary (a2) unless a texture with the same
// name (stored at texture+0x10) is already present. Returns the texture.
static int __cdecl TexDict_AddTextureIfMissing(int textureObj, int dictionary)
{
    const char* textureName = reinterpret_cast<const char*>(static_cast<uintptr_t>(textureObj) + 16);
    if (!RwTexDictionaryFindNamedTexture(dictionary, textureName)) {
        RwTexDictionaryAddTexture(
            reinterpret_cast<void*>(static_cast<uintptr_t>(dictionary)),
            reinterpret_cast<void*>(static_cast<uintptr_t>(textureObj)));
    }
    return textureObj;
}

HOOK(0x6C11C0, TexDict_AddTextureIfMissing, "auto");
