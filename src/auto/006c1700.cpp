#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));
CSF_FUNC(int, __cdecl, RwTexDictionaryCreate, 0x00631650, ());
CSF_FUNC(int, __cdecl, RwTexDictionarySetCurrent, 0x00631990, (int a1));

static int __cdecl ResetCurrentTextureDictionaryCallbackUnused(uint32_t* /*unused*/, int /*unused*/);

static int ResetCurrentTextureDictionary_Callback()
{
    return 0;
}

static int __fastcall ResetCurrentTextureDictionary(C_008607f8* self, void* /*edx*/)
{
    int result = RwTexDictionaryGetCurrent();
    int currentDictionary = result;
    if (currentDictionary != 0)
    {
        RwTexDictionaryForAllTextures(
            currentDictionary,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C1120)),
            0);
        RwTexDictionaryDestroy(currentDictionary);
        int newDictionary = RwTexDictionaryCreate();
        result = RwTexDictionarySetCurrent(newDictionary);
        self->field_0004 = 0;
    }
    return result;
}

HOOK(0x006C1700, ResetCurrentTextureDictionary, "auto");
