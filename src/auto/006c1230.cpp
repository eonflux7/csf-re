#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, RwTexDictionaryRemoveTexture, 0x00631920, (void* texture));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));

// Removes a texture from its dictionary and destroys it when its count field
// (+0x54) is at most 1 (signed compare). Returns the texture argument unchanged.
static int __cdecl TexDict_RemoveAndDestroyLowRefTexture(int a1)
{
    if (a1 && CSF_AT(int, a1, 0x54) <= 1)
    {
        RwTexDictionaryRemoveTexture(reinterpret_cast<void*>(a1));
        RwTextureDestroy(a1);
    }
    return a1;
}

HOOK(0x006C1230, TexDict_RemoveAndDestroyLowRefTexture, "auto");
