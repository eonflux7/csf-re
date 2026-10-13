#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(void*, __cdecl, RwTexDictionaryRemoveTexture, 0x00631920, (void* texture));

// Removes a texture from its dictionary, then destroys it. While the reference
// count at +0x54 is above 1, the texture is destroyed again; the last destroy
// is unconditional. Returns the texture pointer.
static int __cdecl DestroyDictionaryTexture(int a1)
{
    void* texture = reinterpret_cast<void*>(a1);

    RwTexDictionaryRemoveTexture(texture);
    while (CSF_AT(int, texture, 0x54) > 1)
        RwTextureDestroy(a1);
    RwTextureDestroy(a1);
    return a1;
}

HOOK(0x006C1120, DestroyDictionaryTexture, "auto");
