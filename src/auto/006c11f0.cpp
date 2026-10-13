#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));

// Per-texture callback (sub_6C1160), passed by address to RwTexDictionaryForAllTextures.
static int (__cdecl* TextureCallbackFn())(uint32_t*, int) {
    return reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C1160));
}

// Opens a stream on the given source, runs the per-texture callback over every
// texture in the current texture dictionary with that stream as context, then closes it.
// Returns the opened stream handle on success, or 0 if the stream could not be opened.
static int __stdcall GfxTexdict_LoadTexturesFromStreamIntoCurrentDict(int a1)
{
    int result = reinterpret_cast<int>(RwStreamOpen(2, 2, a1));
    int stream = result;
    if (result)
    {
        int context = result;
        int currentDictionary = RwTexDictionaryGetCurrent();
        RwTexDictionaryForAllTextures(currentDictionary, TextureCallbackFn(), context);
        return RwStreamClose(reinterpret_cast<uint32_t*>(stream), nullptr);
    }
    return result;
}

HOOK(0x006C11F0, GfxTexdict_LoadTexturesFromStreamIntoCurrentDict, "auto");
