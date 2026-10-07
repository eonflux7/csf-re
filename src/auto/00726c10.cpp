#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x00726C10 (40 bytes, __stdcall, ret 4)
//
// RwTexture release helper used by the game's texture code: if the texture is
// currently registered in an RwTexDictionary (its "dictionary link" field at
// +0x54 equals 1), unlink it from that dictionary first, then destroy the
// RwTexture object. A null pointer is simply ignored.

CSF_FUNC(void*, __cdecl, RwTexDictionaryRemoveTexture, 0x631920, (void* texture));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x6317E0, (int texture));

static int __stdcall DestroyRwTextureAndUnlink(int texture)
{
    if (texture)
    {
        void* const textureObject = reinterpret_cast<void*>(texture);

        // +0x54 == 1 means the texture still sits in a texture dictionary.
        if (CSF_AT(int, textureObject, 0x54) == 1)
            RwTexDictionaryRemoveTexture(textureObject);

        return RwTextureDestroy(texture);
    }
    return texture;
}

HOOK(0x726C10, DestroyRwTextureAndUnlink, "auto");
