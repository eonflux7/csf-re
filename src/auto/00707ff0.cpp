#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));

static int AnimInterpolator_ForEachTextureInCurrentDict()
{
    int currentDictionary = RwTexDictionaryGetCurrent();
    return RwTexDictionaryForAllTextures(
        currentDictionary,
        reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x00707FC0)),
        0);
}

HOOK(0x707FF0, AnimInterpolator_ForEachTextureInCurrentDict, "auto");
