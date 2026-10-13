#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Texture dictionary (RenderWare) library calls.
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));

// Base class constructor (tail-jumped by the original).
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Per-texture callback used while destroying the dictionary.
static int __cdecl DestroyDictionaryTextureCallback(uint32_t* texture, int context)
{
    return 0;
}

static void __fastcall C_008607f8_ReleaseBase(C_008607f8* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x008607F8);

    int currentDictionary = RwTexDictionaryGetCurrent();
    if (currentDictionary != 0)
    {
        RwTexDictionaryForAllTextures(
            currentDictionary,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C1120)),
            0);
        RwTexDictionaryDestroy(currentDictionary);
    }

    self->field_0004 = 0;
    *(void***)self = CSF_VTBL(0x008607B0);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006C14B0, C_008607f8_ReleaseBase, "auto");
