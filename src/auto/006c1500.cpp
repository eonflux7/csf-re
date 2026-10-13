#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamFindChunk, 0x00629AE0, (uint32_t* a1, int a2, uint32_t* a3, unsigned int* a4));
CSF_FUNC(int, __cdecl, RwTexDictionaryStreamRead, 0x00630A20, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwTexDictionaryCreate, 0x00631650, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionarySetCurrent, 0x00631990, (int a1));
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryDestroy, 0x00632340, (int dictionary));
CSF_FUNC(int, __stdcall, GfxTexdict_ReadDictionaryForMerge, 0x006C1260, (uint32_t* a1, int a2));

static char __fastcall C_008607f8_LoadTextureDictionary(
    C_008607f8* self, void* /*edx*/, uint8_t* fileName, char merge)
{
    int current = RwTexDictionaryGetCurrent();
    if (current && !merge)
    {
        RwTexDictionaryForAllTextures(
            current,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
                CSF_ADDR_FN(0x006C1120)),
            0);
        RwTexDictionaryDestroy(current);
        self->field_0004 = 0;
        current = 0;
    }

    int loaded = 0;
    if (fileName && *fileName)
    {
        uint32_t* stream = RwStreamOpen(2, 1, reinterpret_cast<int>(fileName));
        if (stream)
        {
            if (RwStreamFindChunk(stream, 22, nullptr, nullptr))
            {
                if (merge)
                    loaded = GfxTexdict_ReadDictionaryForMerge(stream, current);
                else
                    loaded = RwTexDictionaryStreamRead(stream);
            }
            RwStreamClose(stream, nullptr);
        }
    }

    if (!loaded)
    {
        if (!merge)
        {
            int emptyDictionary = RwTexDictionaryCreate();
            RwTexDictionarySetCurrent(emptyDictionary);
        }
        return 0;
    }

    if (merge)
    {
        RwTexDictionaryForAllTextures(
            loaded,
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
                CSF_ADDR_FN(0x006C11C0)),
            current);
        RwTexDictionaryDestroy(loaded);
    }
    else
    {
        RwTexDictionarySetCurrent(loaded);
    }

    void** vtable = reinterpret_cast<void**>(self->vtbl);
    self->field_0004 = 0;
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        vtable[0x48 / 4])(self, nullptr);
    return 1;
}

HOOK(0x006C1500, C_008607f8_LoadTextureDictionary, "auto");
